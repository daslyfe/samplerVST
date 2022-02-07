/*
  ==============================================================================

    ADSRComponent.h
    Created: 29 Jan 2022 8:50:09pm
    Author:  Jade Rowland

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UIUtils.h"


//==============================================================================
/*
*/




class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(NewProjectAudioProcessor&);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void updateADSR();
    
private:
    
   
    NewProjectAudioProcessor& audioProcessor;
    // std::vector<InitSlider*> vcaSliders;
    
    InitSlider vcaAtttackSlider, vcaDecaySlider, vcaSustainSlider, vcaReleaseSlider;
    InitSlider* vcaSliders[4] = {&vcaAtttackSlider, &vcaDecaySlider, &vcaSustainSlider, &vcaReleaseSlider};

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
