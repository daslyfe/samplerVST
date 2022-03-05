/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2020 - Raw Material Software Limited

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 6 End-User License
   Agreement and JUCE Privacy Policy (both effective as of the 16th June 2020).

   End User License Agreement: www.juce.com/juce-6-licence
   Privacy Policy: www.juce.com/juce-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "MSamplerVoice.h"

MSamplerSound::MSamplerSound (const juce::String& soundName,
                            juce::AudioFormatReader& source,
                            const juce::BigInteger& notes,
                            int midiNoteForNormalPitch,
                            double attackTimeSecs,
                            double releaseTimeSecs,
                            double maxSampleLengthSeconds)
    : name (soundName),
      sourceSampleRate (source.sampleRate),
      midiNotes (notes),
      midiRootNote (midiNoteForNormalPitch)
{
    if (sourceSampleRate > 0 && source.lengthInSamples > 0)
    {
        length = juce::jmin ((int) source.lengthInSamples,
                       (int) (maxSampleLengthSeconds * sourceSampleRate));

        data.reset (new juce::AudioBuffer<float> (juce::jmin (2, (int) source.numChannels), length + 4));

        source.read (data.get(), 0, length + 4, 0, true, true);

        vcaADSRParams.attack  = static_cast<float> (attackTimeSecs);
        vcaADSRParams.release = static_cast<float> (releaseTimeSecs);
    }
}

MSamplerSound::~MSamplerSound()
{
}

bool MSamplerSound::appliesToNote (int midiNoteNumber)
{
    return midiNotes[midiNoteNumber];
}

bool MSamplerSound::appliesToChannel (int /*midiChannel*/)
{
    return true;
}

//==============================================================================
MSamplerVoice::MSamplerVoice(juce::AudioProcessorValueTreeState* apvts): mAPVTS(apvts) {
   
    
}

MSamplerVoice::~MSamplerVoice() {}

bool MSamplerVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<const MSamplerSound*> (sound) != nullptr;
}


void MSamplerVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    
  
//    reset();
    

   filterADSR.setSampleRate (sampleRate);
    adsr.setSampleRate (sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    for (int ch = 0; ch < numChannelsToProcess; ch++)
    {
        filter[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
        lfo[ch].prepare (spec);
        lfo[ch].initialise ([](float x) { return std::sin (x); });
    }
//
//    gain.prepare (spec);
//    gain.setGainLinear (0.07f);
    isPrepared = true;
}

void MSamplerVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int /*currentPitchWheelPosition*/)
{
    if (auto* sound = dynamic_cast<const MSamplerSound*> (s))
    {
        pitchRatio = std::pow (2.0, (midiNoteNumber - sound->midiRootNote) / 12.0)
                        * sound->sourceSampleRate / getSampleRate();
        sourceSamplePosition =  0;
        lgain = velocity;
        rgain = velocity;

        adsr.setParameters (sound->vcaADSRParams);
        filterADSR.setParameters(filterADSRParams);

        adsr.noteOn();
        filterADSR.noteOn();
        
        
    }
    else
    {
        jassertfalse; // this object can only play MSamplerSounds!
    }
}

void MSamplerVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
        filterADSR.noteOff();
    }
    else
    {
        clearCurrentNote();
        adsr.reset();
        filterADSR.reset();
    }

}

void MSamplerVoice::pitchWheelMoved (int /*newValue*/) {}
void MSamplerVoice::controllerMoved (int /*controllerNumber*/, int /*newValue*/) {}

//==============================================================================
void MSamplerVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared);
    if (auto* playingSound = static_cast<MSamplerSound*> (getCurrentlyPlayingSound().get()))
    {
        
        auto& data = *playingSound->data;
        //DBG(startSample);

        // int startSample2 = 100000;
        int bufferStart = 0;
        //data.clear(0, 100000);
       // int sampleStart2 = (int)round(startSample * data.getNumSamples());
        const float* const inL = data.getReadPointer (0, bufferStart);
        
        
        
       
        
        const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer (1, bufferStart) : nullptr;

        float* outL = outputBuffer.getWritePointer (0, 0);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, 0) : nullptr;
        while (--numSamples >= 0)
        {
            
         
          
            auto pos = (int) sourceSamplePosition;
            auto alpha = (float) (sourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;
            auto envelopeValue = adsr.getNextSample();
            filterADSROutput = filterADSR.getNextSample();

            // just using a very simple linear interpolation here..
            float l = (inL[pos] * invAlpha) + (inL[pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
                                       : l;
//            for (int i = 0; i < 1; ++i) {
                l = filter[0].processNextSample (0, l);
                r = filter[1].processNextSample(1, r);
//            }
            

              

            l *= lgain * envelopeValue;
            r *= rgain * envelopeValue;
            

            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            }
            else
            {
                *outL++ += (l + r) * 0.5f;
            }

            sourceSamplePosition += pitchRatio;
            
//            if (sourceSamplePosition < startSample) {
//                sourceSamplePosition = startSample;
//            }
    
        

            if (sourceSamplePosition > playingSound->length)
            {
                stopNote (0.0f, false);
                break;
            }
        }
        
        
        
        
    }
}

void MSamplerVoice::updateModParams (const int filterType, const float filterCutoff, const float filterResonance, const float adsrDepth, const float lfoFreq, const float lfoDepth)
{
    auto cutoff = (adsrDepth * filterADSROutput) + filterCutoff;
    cutoff = std::min(20000.0f, std::max(cutoff, 20.0f));

   

    for (int ch = 0; ch < numChannelsToProcess; ++ch)
    {
        filter[ch].setParams (filterType, cutoff, filterResonance);
    }
    
//    auto cutoff = (adsrDepth * adsr.getNextSample()) + filterCutoff;
//
//    DBG (cutoff);
//
//    for (int ch = 0; ch < numChannelsToProcess; ++ch)
//    {
//        lfo[ch].setFrequency (lfoFreq);
//        //cutoff = (lfoDepth * lfoOutput[ch]) + cutoff;
//        cutoff = std::clamp<float> (cutoff, 20.0f, 20000.0f);
//        filter[ch].setParams (filterType, cutoff, filterResonance);
//    }
}


//std::tuple<double, Direction> MSamplerVoice::getNextState (double freq,
//                                            double begin,
//                                            double end)
//{
//    auto nextPitchRatio = freq / samplerSound->getCentreFrequencyInHz();
//
//    auto nextSamplePos = currentSamplePos;
//    auto nextDirection = currentDirection;
//
//    // Move the current sample pos in the correct direction
//    switch (currentDirection)
//    {
//        case Direction::forward:
//            nextSamplePos += nextPitchRatio;
//            break;
//
//        case Direction::backward:
//            nextSamplePos -= nextPitchRatio;
//            break;
//
//        default:
//            break;
//    }
//
//    // Update current sample position, taking loop mode into account
//    // If the loop mode was changed while we were travelling backwards, deal
//    // with it gracefully.
//    if (nextDirection == Direction::backward && nextSamplePos < begin)
//    {
//        nextSamplePos = begin;
//        nextDirection = Direction::forward;
//
//        return std::tuple<double, Direction> (nextSamplePos, nextDirection);
//    }
//
//    if (samplerSound->getLoopMode() == LoopMode::none)
//        return std::tuple<double, Direction> (nextSamplePos, nextDirection);
//
//    if (nextDirection == Direction::forward && end < nextSamplePos && !isTailingOff())
//    {
//        if (samplerSound->getLoopMode() == LoopMode::forward)
//            nextSamplePos = begin;
//        else if (samplerSound->getLoopMode() == LoopMode::pingpong)
//        {
//            nextSamplePos = end;
//            nextDirection = Direction::backward;
//        }
//    }
//    return std::tuple<double, Direction> (nextSamplePos, nextDirection);
//}
