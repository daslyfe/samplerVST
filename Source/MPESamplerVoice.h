/*
 ==============================================================================
 
 MPESamplerVoice.h
 Created: 20 Feb 2022 11:01:40pm
 Author:  Jade Rowland
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "MPESamplerSound.h"
#include "FilterData.h"


class MPESamplerVoice  : public juce::MPESynthesiserVoice
{
public:
    explicit MPESamplerVoice (std::shared_ptr<const MPESamplerSound> sound, float& beginSampleRef)
    : samplerSound (std::move (sound)), beginSample(beginSampleRef)
    {
        jassert (samplerSound != nullptr);
        
    }
    
    void noteStarted() override
    {
        jassert (currentlyPlayingNote.isValid());
        jassert (currentlyPlayingNote.keyState == juce::MPENote::keyDown
                 || currentlyPlayingNote.keyState == juce::MPENote::keyDownAndSustained);
        
        level    .setTargetValue (currentlyPlayingNote.noteOnVelocity.asUnsignedFloat());
        frequency.setTargetValue (currentlyPlayingNote.getFrequencyInHertz());
        
        auto loopPoints = samplerSound->getLoopPointsInSeconds();
        loopBegin.setTargetValue (loopPoints.getStart() * samplerSound->getSample()->getSampleRate());
        loopEnd  .setTargetValue (loopPoints.getEnd()   * samplerSound->getSample()->getSampleRate());
        
        for (auto smoothed : { &level, &frequency, &loopBegin, &loopEnd })
            smoothed->reset (currentSampleRate, smoothingLengthInSeconds);
        
        previousPressure = currentlyPlayingNote.pressure.asUnsignedFloat();
        currentSamplePos = (int)(beginSample * samplerSound->getSample()->getLength());
        //currentSamplePos = 0.0;
        tailOff          = 0.0;
        
        
        filterADSR.setParameters(filterADSRParams);
        filterADSR.noteOn();
        
    }
    
    void noteStopped (bool allowTailOff) override
    {
        jassert (currentlyPlayingNote.keyState == juce::MPENote::off);
        filterADSR.noteOff();
        if (allowTailOff && tailOff == 0.0)
            tailOff = 1.0;
        else
            stopNote();
    }
    
    void notePressureChanged() override
    {
        const auto currentPressure = static_cast<double> (currentlyPlayingNote.pressure.asUnsignedFloat());
        const auto deltaPressure = currentPressure - previousPressure;
        level.setTargetValue (juce::jlimit (0.0, 1.0, level.getCurrentValue() + deltaPressure));
        previousPressure = currentPressure;
    }
    
    void notePitchbendChanged() override
    {
        frequency.setTargetValue (currentlyPlayingNote.getFrequencyInHertz());
    }
    
    
    void noteTimbreChanged()   override {}
    void noteKeyStateChanged() override {}
    
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
    {
        
        
        //    reset();
        
        
        //       filterADSR.setSampleRate (sampleRate);
        //        adsr.setSampleRate (sampleRate);
        
        juce::dsp::ProcessSpec spec;
        spec.maximumBlockSize = samplesPerBlock;
        spec.sampleRate = sampleRate;
        spec.numChannels = outputChannels;
        
        for (int ch = 0; ch < numChannelsToProcess; ch++)
        {
            filter[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
            //            lfo[ch].prepare (spec);
            //            lfo[ch].initialise ([](float x) { return std::sin (x); });
        }
        //
        //    gain.prepare (spec);
        //    gain.setGainLinear (0.07f);
        isPrepared = true;
    }
    
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                          int startSample,
                          int numSamples) override
    {
        render (outputBuffer, startSample, numSamples);
    }
    
    void renderNextBlock (juce::AudioBuffer<double>& outputBuffer,
                          int startSample,
                          int numSamples) override
    {
        render (outputBuffer, startSample, numSamples);
    }
    
    void setFilterEnvelopeParameters (juce::ADSR::Parameters parametersToUse)
    {
        filterADSRParams = parametersToUse;
    }
    
    void updateModParams (const int filterType, const float filterCutoff, const float filterResonance, const float adsrDepth, const float lfoFreq, const float lfoDepth)
    {
        auto cutoff = (adsrDepth * filterADSROutput) + filterCutoff;
        cutoff = std::min(20000.0f, std::max(cutoff, 20.0f));
        
        for (int ch = 0; ch < numChannelsToProcess; ++ch)
        {
            filter[ch].setParams (filterType, cutoff, filterResonance);
        }
    }
    
    double getCurrentSamplePosition() const
    {
        return currentSamplePos;
    }
    
private:
    template <typename Element>
    void render (juce::AudioBuffer<Element>& outputBuffer, int startSample, int numSamples)
    {
        jassert (samplerSound->getSample() != nullptr);
        
        auto loopPoints = samplerSound->getLoopPointsInSeconds();
        loopBegin.setTargetValue (loopPoints.getStart() * samplerSound->getSample()->getSampleRate());
        loopEnd  .setTargetValue (loopPoints.getEnd()   * samplerSound->getSample()->getSampleRate());
        
        auto& data = samplerSound->getSample()->getBuffer();
        
        auto inL = data.getReadPointer (0);
        auto inR = data.getNumChannels() > 1 ? data.getReadPointer (1) : nullptr;
        
        auto outL = outputBuffer.getWritePointer (0, startSample);
        
        if (outL == nullptr)
            return;
        
        auto outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, startSample)
        : nullptr;
        
        size_t writePos = 0;
        
        while (--numSamples >= 0 && renderNextSample (inL, inR, outL, outR, writePos))
            writePos += 1;
    }
    
    template <typename Element>
    bool renderNextSample (const float* inL,
                           const float* inR,
                           Element* outL,
                           Element* outR,
                           size_t writePos)
    {
        auto currentLevel     = level.getNextValue();
        auto currentFrequency = frequency.getNextValue();
        auto currentLoopBegin = loopBegin.getNextValue();
        auto currentLoopEnd   = loopEnd.getNextValue();
        
        if (isTailingOff())
        {
            currentLevel *= tailOff;
            tailOff *= 0.9999;
            
            if (tailOff < 0.005)
            {
                stopNote();
                return false;
            }
        }
        
        auto pos      = (int) currentSamplePos;
        auto nextPos  = pos + 1;
        auto alpha    = (Element) (currentSamplePos - pos);
        auto invAlpha = 1.0f - alpha;
        
        // just using a very simple linear interpolation here..
        auto l = static_cast<Element> (currentLevel * (inL[pos] * invAlpha + inL[nextPos] * alpha));
        auto r = static_cast<Element> ((inR != nullptr) ? currentLevel * (inR[pos] * invAlpha + inR[nextPos] * alpha)
                                       : l);
        
        filterADSROutput = filterADSR.getNextSample();
        l = filter[0].processNextSample (0, l);
        r = filter[1].processNextSample(1, r);
        
        
        if (outR != nullptr)
        {
            outL[writePos] += l;
            outR[writePos] += r;
        }
        else
        {
            outL[writePos] += (l + r) * 0.5f;
        }
        
        std::tie (currentSamplePos, currentDirection) = getNextState (currentFrequency,
                                                                      currentLoopBegin,
                                                                      currentLoopEnd);
        
        if (currentSamplePos > samplerSound->getSample()->getLength())
        {
            stopNote();
            return false;
        }
        
        return true;
    }
    
    double getSampleValue() const;
    
    bool isTailingOff() const
    {
        return tailOff != 0.0;
    }
    
    void stopNote()
    {
        clearCurrentNote();
        currentSamplePos = 0.0;
        
        
    }
    
    enum class Direction
    {
        forward,
        backward
    };
    
    std::tuple<double, Direction> getNextState (double freq,
                                                double begin,
                                                double end) const
    {
        auto nextPitchRatio = freq / samplerSound->getCentreFrequencyInHz();
        
        auto nextSamplePos = currentSamplePos;
        auto nextDirection = currentDirection;
        
        // Move the current sample pos in the correct direction
        
        switch (currentDirection)
        {
            case Direction::forward:
                nextSamplePos += nextPitchRatio;
                break;
                
            case Direction::backward:
                nextSamplePos -= nextPitchRatio;
                break;
                
            default:
                break;
        }
        
        // Update current sample position, taking loop mode into account
        // If the loop mode was changed while we were travelling backwards, deal
        // with it gracefully.
        if (nextDirection == Direction::backward && nextSamplePos < begin)
        {
            nextSamplePos = begin;
            nextDirection = Direction::forward;
            
            return std::tuple<double, Direction> (nextSamplePos, nextDirection);
        }
        
        if (samplerSound->getLoopMode() == LoopMode::none)
            return std::tuple<double, Direction> (nextSamplePos, nextDirection);
        
        if (nextDirection == Direction::forward && end < nextSamplePos && !isTailingOff())
        {
            if (samplerSound->getLoopMode() == LoopMode::forward)
                nextSamplePos = begin;
            else if (samplerSound->getLoopMode() == LoopMode::pingpong)
            {
                nextSamplePos = end;
                nextDirection = Direction::backward;
            }
        }
        return std::tuple<double, Direction> (nextSamplePos, nextDirection);
    }
    
    std::shared_ptr<const MPESamplerSound> samplerSound;
    juce::SmoothedValue<double> level { 0 };
    juce::SmoothedValue<double> frequency { 0 };
    juce::SmoothedValue<double> loopBegin;
    juce::SmoothedValue<double> loopEnd;
    double previousPressure { 0 };
    double currentSamplePos { 0 };
    double tailOff { 0 };
    Direction currentDirection { Direction::forward };
    double smoothingLengthInSeconds { 0.01 };
    
    static constexpr int numChannelsToProcess { 2 };
    std::array<FilterData, numChannelsToProcess> filter;
    bool isPrepared {false};
    juce::ADSR filterADSR;
    float filterADSROutput { 0.0f };
    juce::ADSR::Parameters filterADSRParams;
    float& beginSample;
};
