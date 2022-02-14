/*
  ==============================================================================

    FilterADSRComponent.cpp
    Created: 29 Jan 2022 8:50:09pm
    Author:  Jade Rowland

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterADSRComponent.h"
#include "UIUtils.h"



//==============================================================================
FilterADSRComponent::FilterADSRComponent(NewProjectAudioProcessor& p) : audioProcessor (p)
{
    filterAtttackSlider.name = "Attack";
    filterAtttackSlider.attatchmentName = "FILTER_ATTACK";

    
    filterDecaySlider.name = "Decay";
    filterDecaySlider.attatchmentName = "FILTER_DECAY";

    filterSustainSlider.name = "Sustain";
    filterSustainSlider.attatchmentName = "FILTER_SUSTAIN";

    
    filterReleaseSlider.name = "Release";
    filterReleaseSlider.attatchmentName = "FILTER_RELEASE";
   
    
    for (auto slider : filterSliders) {
        makeNewSlider(*slider, audioProcessor.getAPVTS());
        addAndMakeVisible(slider->slider);
    }
    
}


FilterADSRComponent::~FilterADSRComponent()
{
}



void FilterADSRComponent::paint (juce::Graphics& g)
{
   


}





void FilterADSRComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    const float YPos = 0.2f;
    float xOffset = 0.0f;
    
    for (auto slider : filterSliders) {
        slider->slider.setBoundsRelative(xOffset, YPos, slider->dialWidth, slider->dialHeight);
        xOffset += slider->dialWidth;
    }
    
  

}
