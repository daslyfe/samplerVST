/*
  ==============================================================================

    UIUtils.cpp
    Created: 5 Feb 2022 2:06:08pm
    Author:  Jade Rowland

  ==============================================================================
*/

#include "UIUtils.h"
#include <JuceHeader.h>

void initializeKnob(std::string name,  juce::Slider& slider, juce::Label& label)
{

    float knFontSize {10.0f};
    int knTextBoxWidth {40};
    int knTextBoxHeight {20};
    auto knLabelColor {juce::Colours::purple};
    auto knColor {juce::Colours::red};
    
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,  knTextBoxWidth, knTextBoxHeight);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, knColor);

    label.setFont(knFontSize);
    label.setColour(juce::Label::ColourIds::textColourId, knLabelColor);
    label.setText(name, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.attachToComponent(&slider, false);
    

}

void makeNewSlider(InitSlider& sliderObj, juce::AudioProcessorValueTreeState& apvts)
{

    float knFontSize {10.0f};
    int knTextBoxWidth {40};
    int knTextBoxHeight {20};
    auto knLabelColor {juce::Colours::purple};
    auto knColor {juce::Colours::red};
    sliderObj.slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderObj.slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,  knTextBoxWidth, knTextBoxHeight);
    sliderObj.slider.setColour(juce::Slider::ColourIds::thumbColourId, knColor);
    

    sliderObj.label.setFont(knFontSize);
    sliderObj.label.setColour(juce::Label::ColourIds::textColourId, knLabelColor);
    sliderObj.label.setText(sliderObj.name, juce::NotificationType::dontSendNotification);
    sliderObj.label.setJustificationType(juce::Justification::centred);
    sliderObj.label.attachToComponent(&sliderObj.slider, false);
    
    sliderObj.attatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, sliderObj.attatchmentName, sliderObj.slider);
    
    

}
