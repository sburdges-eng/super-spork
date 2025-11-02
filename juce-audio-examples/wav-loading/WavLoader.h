/*
  ==============================================================================

   WAV File Loading Example - JUCE
   Demonstrates how to load WAV files into AudioSampleBuffer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class WavLoader
{
public:
    WavLoader()
    {
        // Register basic audio formats (WAV, AIFF, etc.)
        formatManager.registerBasicFormats();
    }

    //==============================================================================
    /**
     * Load a WAV file into an AudioBuffer
     *
     * @param filePath Path to the WAV file
     * @param buffer The AudioBuffer to load the audio into
     * @return true if successful, false otherwise
     */
    bool loadWavFile(const juce::String& filePath, juce::AudioBuffer<float>& buffer)
    {
        juce::File audioFile(filePath);

        if (!audioFile.existsAsFile())
        {
            DBG("File does not exist: " + filePath);
            return false;
        }

        // Create a reader for the file
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(audioFile));

        if (reader == nullptr)
        {
            DBG("Failed to create reader for file: " + filePath);
            return false;
        }

        // Resize the buffer to match the file's specifications
        buffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);

        // Read the entire file into the buffer
        reader->read(&buffer, 0, (int)reader->lengthInSamples, 0, true, true);

        DBG("Successfully loaded: " + filePath);
        DBG("Sample Rate: " + juce::String(reader->sampleRate));
        DBG("Channels: " + juce::String(reader->numChannels));
        DBG("Length (samples): " + juce::String(reader->lengthInSamples));
        DBG("Duration (seconds): " + juce::String(reader->lengthInSamples / reader->sampleRate));

        return true;
    }

    //==============================================================================
    /**
     * Load a specific portion of a WAV file
     *
     * @param filePath Path to the WAV file
     * @param buffer The AudioBuffer to load the audio into
     * @param startSample Starting sample position
     * @param numSamplesToRead Number of samples to read
     * @return true if successful, false otherwise
     */
    bool loadWavFileSection(const juce::String& filePath,
                            juce::AudioBuffer<float>& buffer,
                            int64 startSample,
                            int numSamplesToRead)
    {
        juce::File audioFile(filePath);

        if (!audioFile.existsAsFile())
            return false;

        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(audioFile));

        if (reader == nullptr)
            return false;

        // Ensure we don't read past the end of the file
        numSamplesToRead = juce::jmin(numSamplesToRead,
                                      (int)(reader->lengthInSamples - startSample));

        if (numSamplesToRead <= 0)
            return false;

        buffer.setSize((int)reader->numChannels, numSamplesToRead);
        reader->read(&buffer, 0, numSamplesToRead, startSample, true, true);

        return true;
    }

    //==============================================================================
    /**
     * Load WAV file using WavAudioFormat directly (alternative method)
     */
    bool loadWavFileDirect(const juce::String& filePath, juce::AudioBuffer<float>& buffer)
    {
        juce::File audioFile(filePath);

        if (!audioFile.existsAsFile())
            return false;

        juce::WavAudioFormat wavFormat;
        std::unique_ptr<juce::AudioFormatReader> reader;

        reader.reset(wavFormat.createReaderFor(audioFile.createInputStream().release(), true));

        if (reader == nullptr)
            return false;

        buffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&buffer, 0, (int)reader->lengthInSamples, 0, true, true);

        return true;
    }

    //==============================================================================
    /**
     * Get audio file information without loading the entire file
     */
    struct AudioFileInfo
    {
        double sampleRate = 0.0;
        int numChannels = 0;
        int64 lengthInSamples = 0;
        double durationSeconds = 0.0;
        int bitsPerSample = 0;
        juce::String formatName;
    };

    bool getAudioFileInfo(const juce::String& filePath, AudioFileInfo& info)
    {
        juce::File audioFile(filePath);

        if (!audioFile.existsAsFile())
            return false;

        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(audioFile));

        if (reader == nullptr)
            return false;

        info.sampleRate = reader->sampleRate;
        info.numChannels = (int)reader->numChannels;
        info.lengthInSamples = reader->lengthInSamples;
        info.durationSeconds = reader->lengthInSamples / reader->sampleRate;
        info.bitsPerSample = (int)reader->bitsPerSample;
        info.formatName = reader->getFormatName();

        return true;
    }

private:
    juce::AudioFormatManager formatManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavLoader)
};
