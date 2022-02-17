/*
  ==============================================================================

    AdsrData.h
    Created: 15 Feb 2022 8:54:32pm
    Author:  Jade Rowland

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AdsrData : public juce::ADSR
{
public:
    void update (const float attack, const float decay, const float sustain, const float release);
    
private:
    juce::ADSR::Parameters adsrParams;
};