/*
  ==============================================================================

    UIUtils.h
    Created: 5 Feb 2022 2:06:08pm
    Author:  Jade Rowland

  ==============================================================================
*/
#include <JuceHeader.h>


#pragma once

class InitSlider {
public:
    juce::Slider slider;
    juce::Label label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attatchment;
    std::string name;
    std::string attatchmentName;
};

void initializeKnob(std::string name,  juce::Slider& slider, juce::Label& label);
void makeNewSlider(InitSlider& sliderObj, juce::AudioProcessorValueTreeState& apvts );
