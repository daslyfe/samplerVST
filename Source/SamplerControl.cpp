/*
  ==============================================================================

    SamplerControl.cpp
    Created: 20 Feb 2022 2:26:12pm
    Author:  Jade Rowland

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SamplerControl.h"

//==============================================================================


SamplerControl::SamplerControl(NewProjectAudioProcessor& p) : audioProcessor (p)
{
    sampleStartSlider.name="Start";
    sampleStartSlider.attatchmentName = "SAMPLE_START";
    
    sampleEndSlider.name = "End";
    sampleEndSlider.attatchmentName = "SAMPLE_END";
    

    for (auto slider : sliders) {
        makeNewSlider(*slider, audioProcessor.getAPVTS());
        addAndMakeVisible(slider->slider);
    }
//
}

SamplerControl::~SamplerControl()
{
}

void SamplerControl::paint (juce::Graphics& g)
{

}

void SamplerControl::resized()
{
    
    const float YPos = 0.2f;
    float xOffset = 0.0f;
    
    for (auto slider : sliders) {
        slider->slider.setBoundsRelative(xOffset, YPos, slider->dialWidth, slider->dialHeight);
        xOffset += slider->dialWidth;
    }
    
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
