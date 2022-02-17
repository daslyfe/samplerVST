/*
  ==============================================================================
    SynthVoice.h
    Created: 10 Dec 2020 1:55:41pm
    Author:  Joshua Hodge
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "FilterData.h"
#include "AdsrData.h"

class SynthVoice : public juce::SamplerVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    
    void reset();
    

    AdsrData& getAdsr() { return adsr; }
    AdsrData& getFilterAdsr() { return filterAdsr; }
    float getFilterAdsrOutput() { return filterAdsrOutput; }
    void updateModParams (const int filterType, const float filterCutoff, const float filterResonance, const float adsrDepth, const float lfoFreq, const float lfoDepth);
    using juce::SamplerVoice::renderNextBlock;
    
private:
    static constexpr int numChannelsToProcess { 2 };
    std::array<FilterData, numChannelsToProcess> filter;
    std::array<juce::dsp::Oscillator<float>, numChannelsToProcess> lfo;
    AdsrData adsr;
    AdsrData filterAdsr;
    juce::AudioBuffer<float> synthBuffer;
    float filterAdsrOutput { 0.0f };
    std::array<float, numChannelsToProcess> lfoOutput { 0.0f, 0.0f };

    
    juce::dsp::Gain<float> gain;
    bool isPrepared { false };
    JUCE_LEAK_DETECTOR(SynthVoice)
};
