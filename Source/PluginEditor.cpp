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
    : AudioProcessorEditor (&p), mWaveThumbnail(p), mADSR(p),  audioProcessor (p)
{

    
    
    addAndMakeVisible(mWaveThumbnail);
    addAndMakeVisible(mADSR);

    
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
    mWaveThumbnail.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);
    mADSR.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}




//void NewProjectAudioProcessorEditor::timerCallback()
//{
//    DBG("hi");
//    // repaint();
//}
