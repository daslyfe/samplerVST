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


class FilterADSRComponent  : public juce::Component
{
public:
    FilterADSRComponent(NewProjectAudioProcessor&);
    ~FilterADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void updateADSR();
    
private:
    
   
    NewProjectAudioProcessor& audioProcessor;
    // std::vector<InitSlider*> filterSliders;
    
    InitSlider filterAtttackSlider, filterDecaySlider, filterSustainSlider, filterReleaseSlider;
    InitSlider* filterSliders[4] = {&filterAtttackSlider, &filterDecaySlider, &filterSustainSlider, &filterReleaseSlider};

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterADSRComponent);
};
