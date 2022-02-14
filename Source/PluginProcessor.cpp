/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        mAPVTS(*this, nullptr, "PARAMETERS", createParameters())
#endif
{
    mFormatManager.registerBasicFormats();
    for (int i=0; i < mNumVoices; i++ ) {
        mSampler.addVoice(new juce::SamplerVoice());
    }
    mAPVTS.state.addListener(this);
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    mFilter.prepare(spec);
   
    
    mFilter.reset();
   
    mFilter.setEnabled(true);
    mFilter.setMode(juce::dsp::LadderFilterMode::LPF24);
   
    
    
    
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    updateADSR();
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
 


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    mSampler.renderNextBlock(buffer, midiMessages, 0,  buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> sampleBlock(buffer);
    mFilter.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));
    
    if (mShouldUpdate)
    {
        updateADSR();
    }
    
    
//    juce::MidiMessage m;
//    juce::MidiBuffer::Iterator it {midiMessages};
//
//    int sample;
//    //initializing midi messages list using a constructor
//    while (it.getNextEvent(m, sample))
//    {
//        if (m.isNoteOn())
//        {
//            mIsNotePlayed = true;
//        }
//        else if (m.isNoteOff())
//        {
//            mIsNotePlayed = false;
//        }
//    }
//
//    mSampleCount = mIsNotePlayed ? mSampleCount += buffer.getNumSamples() : 0;
    
    
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void NewProjectAudioProcessor::loadFileWithMenu()
{
    mSampler.clearSounds();
    juce::FileChooser chooser {"Please load a file"};
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);
    }
    juce::BigInteger range;
    range.setRange(0, 128, true);
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));

    
}

void NewProjectAudioProcessor::loadFile(const juce::String& path)
{
    mSampler.clearSounds();
    auto file = juce::File(path);
    mFormatReader = mFormatManager.createReaderFor(file);
    auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
    mWaveForm.setSize(1, sampleLength);
    mFormatReader->read(&mWaveForm, 0, sampleLength, 0, true, false);

    juce::BigInteger range;
    range.setRange(0, 128, true);
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
    updateADSR();
}

void NewProjectAudioProcessor::updateADSR()
{
    
    // load the pointer address value for the sliders, set the values

    mADSRParams.attack = mAPVTS.getRawParameterValue("ATTACK")->load();
    mADSRParams.decay = mAPVTS.getRawParameterValue("DECAY")->load();
    mADSRParams.sustain = mAPVTS.getRawParameterValue("SUSTAIN")->load();
    mADSRParams.release = mAPVTS.getRawParameterValue("RELEASE")->load();
    mFilter.setResonance(mAPVTS.getRawParameterValue("FILTER_RES")->load());
    mFilter.setCutoffFrequencyHz(mAPVTS.getRawParameterValue("FILTER_CUTOFF")->load());
    mFilter.setDrive(mAPVTS.getRawParameterValue("FILTER_DRIVE")->load());
    
    
    // mFilter.setDrive(1.0f);
    
    for (int i = 0; i < mSampler.getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(i).get()))
        {
            sound->setEnvelopeParameters(mADSRParams);
            
        }
        
    }
    
}
juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createParameters()
{
    //std::vector<std::unique_ptr<juce::RangedAudioParameter>> mAPVTSParams;
    
    mAPVTSParams.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    mAPVTSParams.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 3.0f, 2.0f));
    mAPVTSParams.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    mAPVTSParams.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 0.0f));
    auto filterCutoffRange = juce::NormalisableRange<float>(30.0f, 20000.0f);
    filterCutoffRange.setSkewForCentre(1200.0f);
    
    mAPVTSParams.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_CUTOFF", "Cutoff", filterCutoffRange, 20000.0f));
    mAPVTSParams.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_RES", "Res", 0.0f, 1.0f, 0.0f));
    mAPVTSParams.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_DRIVE", "Drive", 1.0f, 10.0f, 1.0f));
    

    
    return {mAPVTSParams.begin(), mAPVTSParams.end()};
}

void NewProjectAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    mShouldUpdate = true;

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
