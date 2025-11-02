/*
  ==============================================================================

   WAV File Writing Example - JUCE
   Demonstrates how to save AudioSampleBuffer to WAV files

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class WavWriter
{
public:
    WavWriter() = default;

    //==============================================================================
    /**
     * Save an AudioBuffer to a WAV file
     *
     * @param buffer The audio buffer to save
     * @param filePath Output file path
     * @param sampleRate Sample rate of the audio
     * @param bitDepth Bit depth (16 or 24 recommended)
     * @return true if successful, false otherwise
     */
    bool saveToWavFile(const juce::AudioBuffer<float>& buffer,
                       const juce::String& filePath,
                       double sampleRate = 44100.0,
                       int bitDepth = 24)
    {
        juce::File outputFile(filePath);

        // Delete existing file if present
        if (outputFile.existsAsFile())
            outputFile.deleteFile();

        // Create parent directories if they don't exist
        outputFile.getParentDirectory().createDirectory();

        // Create WAV format
        juce::WavAudioFormat wavFormat;

        // Create output stream
        std::unique_ptr<juce::FileOutputStream> fileStream(outputFile.createOutputStream());

        if (fileStream == nullptr)
        {
            DBG("Failed to create output stream for: " + filePath);
            return false;
        }

        // Create the writer
        std::unique_ptr<juce::AudioFormatWriter> writer;
        writer.reset(wavFormat.createWriterFor(fileStream.get(),
                                               sampleRate,
                                               buffer.getNumChannels(),
                                               bitDepth,
                                               {},  // metadata
                                               0)); // quality option (not used for WAV)

        if (writer == nullptr)
        {
            DBG("Failed to create WAV writer");
            return false;
        }

        // Release the stream ownership to the writer
        fileStream.release();

        // Write the buffer to file
        bool success = writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());

        // Writer destructor will flush and close the file
        writer.reset();

        if (success)
        {
            DBG("Successfully saved: " + filePath);
            DBG("Channels: " + juce::String(buffer.getNumChannels()));
            DBG("Samples: " + juce::String(buffer.getNumSamples()));
            DBG("Duration: " + juce::String(buffer.getNumSamples() / sampleRate) + " seconds");
        }

        return success;
    }

    //==============================================================================
    /**
     * Save a portion of an AudioBuffer to a WAV file
     */
    bool saveBufferSection(const juce::AudioBuffer<float>& buffer,
                           const juce::String& filePath,
                           int startSample,
                           int numSamples,
                           double sampleRate = 44100.0,
                           int bitDepth = 24)
    {
        juce::File outputFile(filePath);
        if (outputFile.existsAsFile())
            outputFile.deleteFile();

        juce::WavAudioFormat wavFormat;
        std::unique_ptr<juce::FileOutputStream> fileStream(outputFile.createOutputStream());

        if (fileStream == nullptr)
            return false;

        std::unique_ptr<juce::AudioFormatWriter> writer;
        writer.reset(wavFormat.createWriterFor(fileStream.get(),
                                               sampleRate,
                                               buffer.getNumChannels(),
                                               bitDepth,
                                               {},
                                               0));

        if (writer == nullptr)
            return false;

        fileStream.release();

        // Write only the specified section
        bool success = writer->writeFromAudioSampleBuffer(buffer, startSample, numSamples);
        writer.reset();

        return success;
    }

    //==============================================================================
    /**
     * Save multiple channels to separate mono WAV files
     */
    bool saveChannelsToSeparateFiles(const juce::AudioBuffer<float>& buffer,
                                     const juce::String& baseFilePath,
                                     double sampleRate = 44100.0,
                                     int bitDepth = 24)
    {
        bool allSuccess = true;

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            // Create a mono buffer for this channel
            juce::AudioBuffer<float> monoBuffer(1, buffer.getNumSamples());
            monoBuffer.copyFrom(0, 0, buffer, channel, 0, buffer.getNumSamples());

            // Create filename with channel number
            juce::String filePath = baseFilePath + "_ch" + juce::String(channel + 1) + ".wav";

            if (!saveToWavFile(monoBuffer, filePath, sampleRate, bitDepth))
                allSuccess = false;
        }

        return allSuccess;
    }

    //==============================================================================
    /**
     * Save with custom metadata
     */
    bool saveWithMetadata(const juce::AudioBuffer<float>& buffer,
                          const juce::String& filePath,
                          double sampleRate,
                          int bitDepth,
                          const juce::StringPairArray& metadata)
    {
        juce::File outputFile(filePath);
        if (outputFile.existsAsFile())
            outputFile.deleteFile();

        juce::WavAudioFormat wavFormat;
        std::unique_ptr<juce::FileOutputStream> fileStream(outputFile.createOutputStream());

        if (fileStream == nullptr)
            return false;

        std::unique_ptr<juce::AudioFormatWriter> writer;
        writer.reset(wavFormat.createWriterFor(fileStream.get(),
                                               sampleRate,
                                               buffer.getNumChannels(),
                                               bitDepth,
                                               metadata,  // Add metadata here
                                               0));

        if (writer == nullptr)
            return false;

        fileStream.release();

        bool success = writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        writer.reset();

        return success;
    }

    //==============================================================================
    /**
     * Convert audio to different bit depth while saving
     */
    bool saveWithBitDepthConversion(const juce::AudioBuffer<float>& buffer,
                                    const juce::String& filePath,
                                    double sampleRate,
                                    int targetBitDepth)
    {
        // Supported bit depths: 16, 24, 32
        if (targetBitDepth != 16 && targetBitDepth != 24 && targetBitDepth != 32)
        {
            DBG("Unsupported bit depth: " + juce::String(targetBitDepth));
            return false;
        }

        return saveToWavFile(buffer, filePath, sampleRate, targetBitDepth);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavWriter)
};
