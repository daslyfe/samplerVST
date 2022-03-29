/*
  ==============================================================================

    SamplerControl.h
    Created: 20 Feb 2022 2:26:12pm
    Author:  Jade Rowland

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "UIUtils.h"
#include "PluginProcessor.h"
//==============================================================================
/*
*/
class SamplerControl  : public juce::Component
{
public:
    SamplerControl(NewProjectAudioProcessor&);
    ~SamplerControl() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    NewProjectAudioProcessor& audioProcessor;
    juce::ToggleButton bufferDirectionButton;
    InitSlider sampleStartSlider, sampleEndSlider;
    InitSlider* sliders[2] = {&sampleStartSlider, &sampleEndSlider};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> reverseSampleButtonAttatchment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerControl)
};
