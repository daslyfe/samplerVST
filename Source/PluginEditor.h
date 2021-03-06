/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"
#include "FilterComponent.h"
#include "FilterADSRComponent.h"
#include "SamplerControl.h"


//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
//                                        public juce::Timer
                                       
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

//    void timerCallback() override;
    

private:
    juce::TextButton mLoadButton {"Load"};
    
 
    WaveThumbnail mWaveThumbnail;
    
    ADSRComponent mADSR;
    FilterComponent mFilter;
    FilterADSRComponent mFilterADSR;
    SamplerControl samplerControl;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
