/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 29 Jan 2022 8:50:09pm
    Author:  Jade Rowland

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"
#include "UIUtils.h"



//==============================================================================
ADSRComponent::ADSRComponent(NewProjectAudioProcessor& p) : audioProcessor (p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
//    std::list<juce::Slider> lst1;
//    lst1.push_back(mAttackSlider);

    initializeKnob(mAttackSlider, "Attack");
    addAndMakeVisible(mAttackSlider);
    mAttackAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ATTACK", mAttackSlider);

    mDecaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible(mDecaySlider);
    mDecayLabel.setFont(10.0f);
    mDecayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    mDecayLabel.setJustificationType(juce::Justification::centred);
    mDecayLabel.attachToComponent(&mDecaySlider, false);
    mDecayAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DECAY", mDecaySlider);

    mSustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible(mSustainSlider);
    mSustainLabel.setFont(10.0f);
    mSustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setJustificationType(juce::Justification::centred);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    mSustainAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SUSTAIN", mSustainSlider);
    
    
    mReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible(mReleaseSlider);
    mReleaseLabel.setFont(10.0f);
    mReleaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    mReleaseLabel.setJustificationType(juce::Justification::centred);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);
    mReleaseAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "RELEASE", mReleaseSlider);
    
    

}

ADSRComponent::~ADSRComponent()
{
}



void ADSRComponent::paint (juce::Graphics& g)
{
   


}

void ADSRComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    const float startX = 0.6f;
    const float startY = 0.2f;
    const float dialWidth = 0.1f;
    const float dialHeight = 0.7f;
    
    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + (dialWidth * 2), startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + (dialWidth * 3), startY, dialWidth, dialHeight);

}
