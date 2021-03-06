/*
  ==============================================================================

    FilterComponent.h
    Created: 13 Feb 2022 2:13:09pm
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
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(NewProjectAudioProcessor&);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    
     NewProjectAudioProcessor& audioProcessor;

     
     InitSlider filterDriveSlider, filterCutoffSlider, filterResSlider, filterADSRDepthSlider, filterLFO1DepthSlider;
     InitSlider* sliders[5] = {&filterDriveSlider, &filterCutoffSlider, &filterResSlider, &filterADSRDepthSlider, &filterLFO1DepthSlider};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
