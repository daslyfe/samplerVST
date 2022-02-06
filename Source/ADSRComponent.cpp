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
 
    
    vcaAtttackSlider.name = "Attack";
    vcaAtttackSlider.attatchmentName = "ATTACK";
    makeNewSlider(vcaAtttackSlider, audioProcessor.getAPVTS());
    addAndMakeVisible(vcaAtttackSlider.slider);
    
    vcaDecaySlider.name = "Decay";
    vcaDecaySlider.attatchmentName = "DECAY";
    makeNewSlider(vcaDecaySlider, audioProcessor.getAPVTS());
    addAndMakeVisible(vcaDecaySlider.slider);
    
    vcaSustainSlider.name = "Sustain";
    vcaSustainSlider.attatchmentName = "SUSTAIN";
    makeNewSlider(vcaSustainSlider, audioProcessor.getAPVTS());
    addAndMakeVisible(vcaSustainSlider.slider);
    
    
    vcaReleaseSlider.name = "Release";
    vcaReleaseSlider.attatchmentName = "RELEASE";
    makeNewSlider(vcaReleaseSlider, audioProcessor.getAPVTS());
    addAndMakeVisible(vcaReleaseSlider.slider);
    
    
    

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
