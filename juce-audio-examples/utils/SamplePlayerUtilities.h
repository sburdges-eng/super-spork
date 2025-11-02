/*
  ==============================================================================

   Sample Player Utilities - JUCE
   Helpful utilities for working with audio samples

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Simple sampler that plays audio samples triggered by MIDI
 */
class SimpleSampler : public juce::Synthesiser
{
public:
    SimpleSampler()
    {
        formatManager.registerBasicFormats();

        // Add voices
        for (int i = 0; i < 8; ++i)
            addVoice(new juce::SamplerVoice());
    }

    /**
     * Load a sample and map it to a MIDI note
     */
    bool loadSample(const juce::String& filePath, int midiNote, int midiNoteRangeStart, int midiNoteRangeEnd)
    {
        juce::File audioFile(filePath);

        if (!audioFile.existsAsFile())
            return false;

        auto* reader = formatManager.createReaderFor(audioFile);

        if (reader == nullptr)
            return false;

        juce::BigInteger allNotes;
        allNotes.setRange(midiNoteRangeStart, midiNoteRangeEnd - midiNoteRangeStart + 1, true);

        addSound(new juce::SamplerSound(
            audioFile.getFileNameWithoutExtension(),
            *reader,
            allNotes,
            midiNote,      // root note
            0.01,          // attack time
            0.01,          // release time
            10.0           // max sample length in seconds
        ));

        delete reader;
        return true;
    }

    /**
     * Load a sample mapped to a single MIDI note
     */
    bool loadSample(const juce::String& filePath, int midiNote)
    {
        return loadSample(filePath, midiNote, midiNote, midiNote);
    }

    /**
     * Clear all loaded samples
     */
    void clearSamples()
    {
        clearSounds();
    }

private:
    juce::AudioFormatManager formatManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleSampler)
};

//==============================================================================
/**
 * Drum machine that maps samples to specific MIDI notes
 */
class SimpleDrumMachine : public SimpleSampler
{
public:
    enum DrumType
    {
        Kick = 36,      // C1
        Snare = 38,     // D1
        ClosedHat = 42, // F#1
        OpenHat = 46,   // A#1
        Clap = 39,      // D#1
        Tom1 = 50,      // D2
        Tom2 = 48,      // C2
        Tom3 = 45       // A1
    };

    /**
     * Load drum samples
     */
    bool loadDrumSample(const juce::String& filePath, DrumType drumType)
    {
        return loadSample(filePath, (int)drumType);
    }

    /**
     * Trigger a drum sound
     */
    void triggerDrum(DrumType drumType, float velocity = 1.0f)
    {
        noteOn(1, (int)drumType, (uint8)(velocity * 127.0f));

        // Auto-release after 10ms
        juce::Timer::callAfterDelay(10, [this, drumType]()
        {
            noteOff(1, (int)drumType, 0.0f, true);
        });
    }
};

//==============================================================================
/**
 * Audio buffer utilities
 */
class AudioBufferUtilities
{
public:
    /**
     * Normalize audio buffer to target peak level
     */
    static void normalize(juce::AudioBuffer<float>& buffer, float targetPeak = 1.0f)
    {
        float maxLevel = 0.0f;

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            const float* channelData = buffer.getReadPointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                maxLevel = juce::jmax(maxLevel, std::abs(channelData[sample]));
            }
        }

        if (maxLevel > 0.0f)
        {
            float gainFactor = targetPeak / maxLevel;
            buffer.applyGain(gainFactor);
        }
    }

    /**
     * Apply fade in to buffer
     */
    static void fadeIn(juce::AudioBuffer<float>& buffer, int numSamples)
    {
        numSamples = juce::jmin(numSamples, buffer.getNumSamples());

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.applyGainRamp(channel, 0, numSamples, 0.0f, 1.0f);
        }
    }

    /**
     * Apply fade out to buffer
     */
    static void fadeOut(juce::AudioBuffer<float>& buffer, int numSamples)
    {
        numSamples = juce::jmin(numSamples, buffer.getNumSamples());
        int startSample = buffer.getNumSamples() - numSamples;

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.applyGainRamp(channel, startSample, numSamples, 1.0f, 0.0f);
        }
    }

    /**
     * Reverse audio buffer
     */
    static void reverse(juce::AudioBuffer<float>& buffer)
    {
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);
            std::reverse(channelData, channelData + buffer.getNumSamples());
        }
    }

    /**
     * Mix two buffers together
     */
    static void mix(juce::AudioBuffer<float>& destination,
                    const juce::AudioBuffer<float>& source,
                    int destStartSample = 0,
                    float sourceGain = 1.0f)
    {
        for (int channel = 0; channel < juce::jmin(destination.getNumChannels(), source.getNumChannels()); ++channel)
        {
            destination.addFrom(channel, destStartSample, source, channel, 0,
                              juce::jmin(source.getNumSamples(),
                                       destination.getNumSamples() - destStartSample),
                              sourceGain);
        }
    }

    /**
     * Convert stereo to mono
     */
    static juce::AudioBuffer<float> convertToMono(const juce::AudioBuffer<float>& stereoBuffer)
    {
        juce::AudioBuffer<float> monoBuffer(1, stereoBuffer.getNumSamples());

        if (stereoBuffer.getNumChannels() == 1)
        {
            monoBuffer.copyFrom(0, 0, stereoBuffer, 0, 0, stereoBuffer.getNumSamples());
        }
        else
        {
            // Mix stereo channels to mono
            monoBuffer.copyFrom(0, 0, stereoBuffer, 0, 0, stereoBuffer.getNumSamples());
            monoBuffer.addFrom(0, 0, stereoBuffer, 1, 0, stereoBuffer.getNumSamples(), 0.5f);
            monoBuffer.applyGain(0.5f);
        }

        return monoBuffer;
    }

    /**
     * Resample audio buffer to different sample rate
     */
    static juce::AudioBuffer<float> resample(const juce::AudioBuffer<float>& input,
                                             double sourceSampleRate,
                                             double targetSampleRate)
    {
        if (sourceSampleRate == targetSampleRate)
            return input;

        double ratio = targetSampleRate / sourceSampleRate;
        int newLength = (int)(input.getNumSamples() * ratio);

        juce::AudioBuffer<float> output(input.getNumChannels(), newLength);

        juce::LagrangeInterpolator interpolator;

        for (int channel = 0; channel < input.getNumChannels(); ++channel)
        {
            interpolator.process(ratio,
                                input.getReadPointer(channel),
                                output.getWritePointer(channel),
                                newLength);
            interpolator.reset();
        }

        return output;
    }
};

//==============================================================================
/**
 * Sample library manager
 */
class SampleLibrary
{
public:
    struct Sample
    {
        juce::String name;
        juce::AudioBuffer<float> buffer;
        double sampleRate;
        int rootNote;
    };

    /**
     * Add a sample to the library
     */
    bool addSample(const juce::String& name, const juce::String& filePath, int rootNote = 60)
    {
        juce::File audioFile(filePath);
        if (!audioFile.existsAsFile())
            return false;

        auto* reader = formatManager.createReaderFor(audioFile);
        if (reader == nullptr)
            return false;

        Sample sample;
        sample.name = name;
        sample.sampleRate = reader->sampleRate;
        sample.rootNote = rootNote;
        sample.buffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&sample.buffer, 0, (int)reader->lengthInSamples, 0, true, true);

        delete reader;

        samples.add(sample);
        return true;
    }

    /**
     * Get sample by name
     */
    const Sample* getSample(const juce::String& name) const
    {
        for (const auto& sample : samples)
        {
            if (sample.name == name)
                return &sample;
        }
        return nullptr;
    }

    /**
     * Get all sample names
     */
    juce::StringArray getSampleNames() const
    {
        juce::StringArray names;
        for (const auto& sample : samples)
            names.add(sample.name);
        return names;
    }

    /**
     * Clear all samples
     */
    void clear()
    {
        samples.clear();
    }

    int getNumSamples() const { return samples.size(); }

private:
    juce::Array<Sample> samples;
    juce::AudioFormatManager formatManager;
};
