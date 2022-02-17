/*
  ==============================================================================

    FilterData.h
    Created: 15 Feb 2022 8:56:08pm
    Author:  Jade Rowland

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>


class FilterData : public juce::dsp::StateVariableTPTFilter<float>
{
public:
    FilterData();
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void setParams (const int filterType, const float filterCutoff, const float filterResonance);
    void setLfoParams (const float freq, const float depth);
    void processNextBlock (juce::AudioBuffer<float>& buffer);
    float processNextSample (int channel, float inputValue);
    void resetAll();
    
private:
    void selectFilterType (const int type);
    juce::dsp::Oscillator<float> lfo { [](float x) { return std::sin (x); }};
};
