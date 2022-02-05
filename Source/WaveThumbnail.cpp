/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 29 Jan 2022 11:40:45am
    Author:  Jade Rowland

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(NewProjectAudioProcessor& p) : audioProcessor (p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WaveThumbnail::~WaveThumbnail()
{
}

void WaveThumbnail::paint (juce::Graphics& g)
{
  
    g.fillAll(juce::Colours::cadetblue.darker());
    
    auto waveForm = audioProcessor.getWaveForm();

    
    if (waveForm.getNumSamples() > 0)
    {
        juce::Path p;
        
        auto ratio = waveForm.getNumSamples()/getWidth();
        auto buffer = waveForm.getReadPointer(0);
        
        // decrease the number of samples in the buffer to the set pixel width of the window
        for (int sample = 0; sample < waveForm.getNumSamples(); sample+=ratio )
        {
            mAudioPoints.push_back(buffer[sample]);
        }
        g.setColour(juce::Colours::yellow);
        
        p.startNewSubPath(0, getHeight()/2);
        // clamp the values of each sample in the modified buffer to the height of the window
        for (int sample = 0; sample < mAudioPoints.size(); ++ sample)
        {
            auto point = juce::jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, getHeight(), 0);
            p.lineTo(sample, point);
        }
        
        g.strokePath(p, juce::PathStrokeType(2));
        
       g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        auto textBounds = getLocalBounds().reduced(10, 10);
        g.drawFittedText(mFileName, textBounds, juce::Justification::topRight, 1.0f);
        
      
//        auto playHeadPosition = juce::jmap<int>(audioProcessor.getSampleCount(), 0, audioProcessor.getWaveForm().getNumSamples(), 0, getWidth());
//
//        g.setColour(juce::Colours::white);
//        g.drawLine(playHeadPosition, 0, playHeadPosition, getHeight(), 2.0f);
//        g.setColour(juce::Colours::black.withAlpha(0.2f));
//        g.fillRect(0,0, playHeadPosition, getHeight());
        
    }
    
    else
    {
        
        g.setColour(juce::Colours::white);
        g.setFont(40.0f);
        g.drawFittedText("Drop an audio file to load", getLocalBounds(), juce::Justification::centred, 1);
    }

}

void WaveThumbnail::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
bool WaveThumbnail::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    }
    return false;
}
 
void WaveThumbnail::filesDropped(const juce::StringArray& files, int x, int y)
{
   for (auto file : files)
   {
       if (isInterestedInFileDrag(file))
       {
           auto myFile = std::make_unique<juce::File>(file);
           mFileName = myFile->getFileNameWithoutExtension();
           
           mAudioPoints.clear();
        
           // load file
           audioProcessor.loadFile(file);
        }
    }
  repaint();
}
