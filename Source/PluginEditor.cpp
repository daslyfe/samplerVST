/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
// constructor
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
// instantiation list
    : AudioProcessorEditor (&p), mWaveThumbnail(p), mADSR(p), mFilter(p), mFilterADSR(p), samplerControl(p), audioProcessor (p)
{

    
    
    addAndMakeVisible(mWaveThumbnail);
    addAndMakeVisible(mADSR);
    addAndMakeVisible(mFilter);
    addAndMakeVisible(mFilterADSR);
    addAndMakeVisible(samplerControl);

    
//juce:startTimerHz(30);
   
    setSize (600, 400);

    
    
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
//    stopTimer();
}



//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
  
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);

    


    
}





void NewProjectAudioProcessorEditor::resized()
{
    const float dialComponentWidth = 0.5f;
    const float dialComponentHeight = 0.25f;
    mWaveThumbnail.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);
    mADSR.setBoundsRelative(0.0f, 0.0f, dialComponentWidth, dialComponentHeight);
    mFilter.setBoundsRelative(0.5f, 0.75f, dialComponentWidth,  dialComponentHeight);
    mFilterADSR.setBoundsRelative(0.0f, 0.75f, dialComponentWidth, dialComponentHeight);
    samplerControl.setBoundsRelative(0.5f, 0.0f,dialComponentWidth, dialComponentHeight);

    
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}




//void NewProjectAudioProcessorEditor::timerCallback()
//{
//    DBG("hi");
//    // repaint();
//}
