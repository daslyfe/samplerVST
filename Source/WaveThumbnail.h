/*
  ==============================================================================

    WaveThumbnail.h
    Created: 29 Jan 2022 11:40:45am
    Author:  Jade Rowland

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class WaveThumbnail  : public juce::Component,
                         public juce::FileDragAndDropTarget
{
public:
    WaveThumbnail(NewProjectAudioProcessor&);
    ~WaveThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;

private:
    std::vector<float> mAudioPoints;
    juce::String mFileName {""};
    bool mShouldBePainting {false};
    NewProjectAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
