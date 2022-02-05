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
    
   juce::Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    
    
 
    // juce::Slider* sliders [1] = {mAttackSlider};
    juce::Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mAttackAttatchment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDecayAttatchment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSustainAttatchment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mReleaseAttatchment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
