/*
  ==============================================================================

    UIUtils.h
    Created: 5 Feb 2022 2:06:08pm
    Author:  Jade Rowland

  ==============================================================================
*/
#include <JuceHeader.h>


#pragma once

class InitSlider {
public:
    juce::Slider slider;
    juce::Label label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attatchment;
    std::string name;
    std::string attatchmentName;
    float minValue {0.0f};
    float maxValue {1.0f};
    float defaultValue {0.0f};
    float dialWidth  {0.1f};
    float dialHeight {0.7f};
    juce::Colour labelColor {juce::Colours::white};
    juce::Colour color {juce::Colours::red};
};




// InitSlider vcaAtttackSlider, vcaDecaySlider, vcaSustainSlider, vcaReleaseSlider;

juce::AudioProcessorValueTreeState::ParameterLayout createParameters();


void makeNewSlider(InitSlider& sliderObj, juce::AudioProcessorValueTreeState& apvts );
void attatchSlidersToAPVTS(juce::AudioProcessorValueTreeState& apvts);

//class MakeApvts : public  juce::ValueTree::Listener
//{
//
//public:
//    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
//    juce::AudioProcessorValueTreeState& getAPVTS() {return mAPVTS;}
//
//private:
//
//    juce::AudioProcessorValueTreeState mAPVTS;
//
//    void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;
//};
