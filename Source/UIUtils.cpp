/*
  ==============================================================================

    UIUtils.cpp
    Created: 5 Feb 2022 2:06:08pm
    Author:  Jade Rowland

  ==============================================================================
*/

#include "UIUtils.h"
#include <JuceHeader.h>

void initializeKnob(juce::Slider& slider, std::string label)
{
    juce::Label sliderLabel;
    float knFontSize {10.0f};
    int knTextBoxWidth {40};
    int knTextBoxHeight {20};
    auto knLabelColor {juce::Colours::purple};
    auto knColor {juce::Colours::red};
    
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,  knTextBoxWidth, knTextBoxHeight);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, knColor);

    sliderLabel.setFont(knFontSize);
    sliderLabel.setColour(juce::Label::ColourIds::textColourId, knLabelColor);
    sliderLabel.setText("bbbbfbf", juce::NotificationType::dontSendNotification);
    sliderLabel.setJustificationType(juce::Justification::centred);
    sliderLabel.attachToComponent(&slider, false);
}
