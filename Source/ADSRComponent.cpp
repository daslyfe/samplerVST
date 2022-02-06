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

    vcaAtttackSlider.name = "Attack";
    vcaAtttackSlider.attatchmentName = "ATTACK";
    vcaSliders.push_back(&vcaAtttackSlider);
    
    vcaDecaySlider.name = "Decay";
    vcaDecaySlider.attatchmentName = "DECAY";
    vcaSliders.push_back(&vcaDecaySlider);
    
    vcaSustainSlider.name = "Sustain";
    vcaSustainSlider.attatchmentName = "SUSTAIN";
    vcaSliders.push_back(&vcaSustainSlider);
    
    vcaReleaseSlider.name = "Release";
    vcaReleaseSlider.attatchmentName = "RELEASE";
    vcaSliders.push_back(&vcaReleaseSlider);
    
    for (auto slider : vcaSliders) {
        makeNewSlider(*slider, audioProcessor.getAPVTS());
        addAndMakeVisible(slider->slider);
    }
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
    
    vcaAtttackSlider.slider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    vcaDecaySlider.slider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    vcaSustainSlider.slider.setBoundsRelative(startX + (dialWidth * 2), startY, dialWidth, dialHeight);
    vcaReleaseSlider.slider.setBoundsRelative(startX + (dialWidth * 3), startY, dialWidth, dialHeight);

}
