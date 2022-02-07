/*
  ==============================================================================

    UIUtils.cpp
    Created: 5 Feb 2022 2:06:08pm
    Author:  Jade Rowland

  ==============================================================================
*/

#include "UIUtils.h"
#include <JuceHeader.h>



void makeNewSlider(InitSlider& sliderObj, juce::AudioProcessorValueTreeState& apvts)
{

    float knFontSize {14.0f};
    int knTextBoxWidth {40};
    int knTextBoxHeight {20};

    sliderObj.slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderObj.slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,  knTextBoxWidth, knTextBoxHeight);
    sliderObj.slider.setColour(juce::Slider::ColourIds::thumbColourId, sliderObj.color);
    sliderObj.label.setFont(knFontSize);
    sliderObj.label.setColour(juce::Label::ColourIds::textColourId, sliderObj.labelColor);
    sliderObj.label.setText(sliderObj.name, juce::NotificationType::dontSendNotification);
    sliderObj.label.setJustificationType(juce::Justification::centred);
    sliderObj.label.attachToComponent(&sliderObj.slider, false);
    
    sliderObj.attatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, sliderObj.attatchmentName, sliderObj.slider);
    
    

}


juce::AudioProcessorValueTreeState::ParameterLayout createParametersFromSliders(std::vector<InitSlider*> sliders) {
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> mAPVTSParams;
    for (auto slider : sliders) {
        mAPVTSParams.push_back(std::make_unique<juce::AudioParameterFloat>(slider->attatchmentName, slider->name, slider->minValue, slider->maxValue, slider->defaultValue));
    }
    
 
    return {mAPVTSParams.begin(), mAPVTSParams.end()};
}
