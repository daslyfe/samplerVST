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
    vcaAtttackSlider.maxValue = 5.0f;
    
    vcaDecaySlider.name = "Decay";
    vcaDecaySlider.attatchmentName = "DECAY";
    vcaDecaySlider.maxValue = 3.0f;
    vcaDecaySlider.defaultValue = 2.0f;

    vcaSustainSlider.name = "Sustain";
    vcaSustainSlider.attatchmentName = "SUSTAIN";
    vcaSustainSlider.defaultValue = 1.0f;
    
    vcaReleaseSlider.name = "Release";
    vcaReleaseSlider.attatchmentName = "RELEASE";
    vcaReleaseSlider.maxValue = 5.0f;
    
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
    
    const float YPos = 0.2f;
    float xOffset = 0.6f;
    
    for (auto slider : vcaSliders) {
        slider->slider.setBoundsRelative(xOffset, YPos, slider->dialWidth, slider->dialHeight);
        xOffset += slider->dialWidth;
    }
    
  

}
