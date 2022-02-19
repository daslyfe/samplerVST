
#pragma once

#include <JuceHeader.h>
#include "FilterData.h"


//==============================================================================
/**
    A subclass of SynthesiserSound that represents a sampled audio clip.

    This is a pretty basic sampler, and just attempts to load the whole audio stream
    into memory.

    To use it, create a Synthesiser, add some MSamplerVoice objects to it, then
    give it some SampledSound objects to play.

    @see MSamplerVoice, Synthesiser, SynthesiserSound

    @tags{Audio}
*/
class MSamplerSound  : public juce::SynthesiserSound
{
public:
    //==============================================================================
    /** Creates a sampled sound from an audio reader.

        This will attempt to load the audio from the source into memory and store
        it in this object.

        @param name         a name for the sample
        @param source       the audio to load. This object can be safely deleted by the
                            caller after this constructor returns
        @param midiNotes    the set of midi keys that this sound should be played on. This
                            is used by the SynthesiserSound::appliesToNote() method
        @param midiNoteForNormalPitch   the midi note at which the sample should be played
                                        with its natural rate. All other notes will be pitched
                                        up or down relative to this one
        @param attackTimeSecs   the attack (fade-in) time, in seconds
        @param releaseTimeSecs  the decay (fade-out) time, in seconds
        @param maxSampleLengthSeconds   a maximum length of audio to read from the audio
                                        source, in seconds
    */
    MSamplerSound (const juce::String& name,
                  juce::AudioFormatReader& source,
                  const juce::BigInteger& midiNotes,
                  int midiNoteForNormalPitch,
                  double attackTimeSecs,
                  double releaseTimeSecs,
                  double maxSampleLengthSeconds);

    /** Destructor. */
    ~MSamplerSound() override;

    //==============================================================================
    /** Returns the sample's name */
    const juce::String& getName() const noexcept                  { return name; }

    /** Returns the audio sample data.
        This could return nullptr if there was a problem loading the data.
    */
    juce::AudioBuffer<float>* getAudioData() const noexcept       { return data.get(); }

    //==============================================================================
    /** Changes the parameters of the ADSR envelope which will be applied to the sample. */
    void setVcaEnvelopeParameters (juce::ADSR::Parameters parametersToUse)    { vcaADSRParams = parametersToUse; }

    //==============================================================================
    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;

private:
    //==============================================================================
    friend class MSamplerVoice;

    juce::String name;
    std::unique_ptr<juce::AudioBuffer<float>> data;
    double sourceSampleRate;
    juce::BigInteger midiNotes;
    int length = 0, midiRootNote = 0;

    juce::ADSR::Parameters vcaADSRParams;

    

    JUCE_LEAK_DETECTOR (MSamplerSound)
};


//==============================================================================
/**
    A subclass of SynthesiserVoice that can play a MSamplerSound.

    To use it, create a Synthesiser, add some MSamplerVoice objects to it, then
    give it some SampledSound objects to play.

    @see MSamplerSound, Synthesiser, SynthesiserVoice

    @tags{Audio}
*/
class  MSamplerVoice    : public juce::SynthesiserVoice
{
public:
    //==============================================================================
    /** Creates a MSamplerVoice. */
    MSamplerVoice();

    /** Destructor. */
    ~MSamplerVoice() override;

    //==============================================================================
    bool canPlaySound (juce::SynthesiserSound*) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);

    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void setFilterEnvelopeParameters (juce::ADSR::Parameters parametersToUse)    { filterADSRParams = parametersToUse; }
    void pitchWheelMoved (int newValue) override;
    void controllerMoved (int controllerNumber, int newValue) override;
    float getFilterADSROutput() { return filterADSROutput; }
    
    void updateModParams (const int filterType, const float filterCutoff, const float filterResonance, const float adsrDepth, const float lfoFreq, const float lfoDepth);
    void renderNextBlock (juce::AudioBuffer<float>&, int startSample, int numSamples) override;
    using SynthesiserVoice::renderNextBlock;
    

private:
    //==============================================================================
    static constexpr int numChannelsToProcess { 2 };
    double pitchRatio = 0;
    double sourceSamplePosition = 0;
    float lgain = 0, rgain = 0;
    bool isPrepared { false };
    juce::ADSR filterADSR;
    juce::ADSR adsr;
    juce::ADSR::Parameters filterADSRParams;
    std::array<FilterData, numChannelsToProcess> filter;
    std::array<juce::dsp::Oscillator<float>, numChannelsToProcess> lfo;
    float filterADSROutput { 0.0f };

    JUCE_LEAK_DETECTOR (MSamplerVoice)
};


