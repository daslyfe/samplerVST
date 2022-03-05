/*
  ==============================================================================

    MPESamplerSound.h
    Created: 20 Feb 2022 11:01:50pm
    Author:  Jade Rowland

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


//==============================================================================

enum class LoopMode
{
    none,
    forward,
    pingpong
};

// Represents the constant parts of an audio sample: its name, sample rate,
// length, and the audio sample data itself.
// Samples might be pretty big, so we'll keep shared_ptrs to them most of the
// time, to reduce duplication and copying.
class Sample final
{
public:
    Sample (juce::AudioFormatReader& source, double maxSampleLengthSecs)
        : sourceSampleRate (source.sampleRate),
          length (juce::jmin (int (source.lengthInSamples),
                        int (maxSampleLengthSecs * sourceSampleRate))),
          data (juce::jmin (2, int (source.numChannels)), length + 4)
    {
        if (length == 0)
            throw std::runtime_error ("Unable to load sample");

        source.read (&data, 0, length + 4, 0, true, true);
        
    }

    double getSampleRate() const                    { return sourceSampleRate; }
    int getLength() const                           { return length; }
    const juce::AudioBuffer<float>& getBuffer() const     { return data; }

private:
    double sourceSampleRate;
    int length;
    juce::AudioBuffer<float> data;
};

class MPESamplerSound final
{
public:
    void setSample (std::unique_ptr<Sample> value)
    {
        sample = move (value);
        setLoopPointsInSeconds (loopPoints);
    }

    Sample* getSample() const
    {
        return sample.get();
    }

    void setLoopPointsInSeconds (juce::Range<double> value)
    {
        loopPoints = sample == nullptr ? value
                                       : juce::Range<double> (0, sample->getLength() / sample->getSampleRate())
                                                        .constrainRange (value);
    }

    juce::Range<double> getLoopPointsInSeconds() const
    {
        return loopPoints;
    }

    void setCentreFrequencyInHz (double centre)
    {
        centreFrequencyInHz = centre;
    }

    double getCentreFrequencyInHz() const
    {
        return centreFrequencyInHz;
    }

    void setLoopMode (LoopMode type)
    {
        loopMode = type;
    }

    LoopMode getLoopMode() const
    {
        return loopMode;
    }

private:
    std::unique_ptr<Sample> sample;
    double centreFrequencyInHz { 440.0 };
    juce::Range<double> loopPoints;
    LoopMode loopMode { LoopMode::none };
};
