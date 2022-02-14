/*
  ==============================================================================

    FilterComponent.cpp
    Created: 13 Feb 2022 2:13:09pm
    Author:  Jade Rowland

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(NewProjectAudioProcessor& p) : audioProcessor (p)
   {
    
    
    filterCutoffSlider.name = "Cutoff";
    filterCutoffSlider.attatchmentName = "FILTER_CUTOFF";
       // filterCutoffSlider.slider.setRange(30, 20000);

//       filterCutoffSlider.slider.setSkewFactor(0.2);
//       filterCutoffSlider.slider.setSkewFactorFromMidPoint(500);
       
       filterResSlider.name = "Res";
       filterResSlider.attatchmentName = "FILTER_RES";

       filterDriveSlider.name = "Drive";
       filterDriveSlider.attatchmentName = "FILTER_DRIVE";
      
    for (auto slider : sliders) {
        makeNewSlider(*slider, audioProcessor.getAPVTS());
        addAndMakeVisible(slider->slider);
    }
       
     

}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    
}

void FilterComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    const float YPos = 0.2f;
    float xOffset = 0.0f;
    
    for (auto slider : sliders) {
        slider->slider.setBoundsRelative(xOffset, YPos, slider->dialWidth, slider->dialHeight);
        xOffset += slider->dialWidth;
    }


}
