/*
  ==============================================================================

   WAV File Writing Example - Usage Demo

  ==============================================================================
*/

#include "WavWriter.h"

//==============================================================================
// Example 1: Basic WAV file writing
void exampleBasicWriting()
{
    WavWriter writer;

    // Create a simple audio buffer (e.g., a sine wave)
    const int sampleRate = 44100;
    const int durationSeconds = 2;
    const int numSamples = sampleRate * durationSeconds;

    juce::AudioBuffer<float> buffer(2, numSamples); // Stereo

    // Generate a 440 Hz sine wave (A note)
    const float frequency = 440.0f;
    for (int sample = 0; sample < numSamples; ++sample)
    {
        float sineValue = std::sin(2.0f * juce::MathConstants<float>::pi *
                                   frequency * sample / sampleRate);

        buffer.setSample(0, sample, sineValue * 0.5f); // Left channel
        buffer.setSample(1, sample, sineValue * 0.5f); // Right channel
    }

    // Save to file
    writer.saveToWavFile(buffer, "/path/to/output/sine_wave.wav", sampleRate, 24);
}

//==============================================================================
// Example 2: Save a section of audio
void exampleSaveSection()
{
    WavWriter writer;
    juce::AudioBuffer<float> buffer(2, 88200); // 2 seconds at 44.1kHz

    // ... fill buffer with audio data ...

    // Save only the first second
    writer.saveBufferSection(buffer, "/path/to/output/first_second.wav",
                            0, 44100, 44100.0, 24);

    // Save only the second second
    writer.saveBufferSection(buffer, "/path/to/output/second_second.wav",
                            44100, 44100, 44100.0, 24);
}

//==============================================================================
// Example 3: Process and save recorded audio
void exampleProcessAndSave()
{
    WavWriter writer;

    // Assume we have recorded audio in a buffer
    juce::AudioBuffer<float> recordedBuffer(2, 88200);

    // ... record audio into buffer ...

    // Apply some processing (e.g., normalization)
    float maxLevel = 0.0f;
    for (int channel = 0; channel < recordedBuffer.getNumChannels(); ++channel)
    {
        const float* channelData = recordedBuffer.getReadPointer(channel);
        for (int sample = 0; sample < recordedBuffer.getNumSamples(); ++sample)
        {
            maxLevel = juce::jmax(maxLevel, std::abs(channelData[sample]));
        }
    }

    if (maxLevel > 0.0f)
    {
        float gainFactor = 0.9f / maxLevel; // Normalize to -0.9 dB peak
        recordedBuffer.applyGain(gainFactor);
    }

    // Save the processed audio
    writer.saveToWavFile(recordedBuffer, "/path/to/output/normalized.wav", 44100.0, 24);
}

//==============================================================================
// Example 4: Save channels to separate files
void exampleSaveSeparateChannels()
{
    WavWriter writer;

    // Create a multi-channel buffer
    juce::AudioBuffer<float> multiChannelBuffer(4, 44100); // 4 channels, 1 second

    // ... fill with different audio data per channel ...

    // Save each channel as a separate mono file
    // This will create: output_ch1.wav, output_ch2.wav, output_ch3.wav, output_ch4.wav
    writer.saveChannelsToSeparateFiles(multiChannelBuffer,
                                       "/path/to/output/output",
                                       44100.0,
                                       24);
}

//==============================================================================
// Example 5: Save with metadata
void exampleSaveWithMetadata()
{
    WavWriter writer;
    juce::AudioBuffer<float> buffer(2, 88200);

    // ... fill buffer with audio data ...

    // Add metadata
    juce::StringPairArray metadata;
    metadata.set("ISFT", "JUCE Example");           // Software
    metadata.set("IART", "Artist Name");            // Artist
    metadata.set("INAM", "Song Title");             // Title
    metadata.set("ICMT", "Generated with JUCE");    // Comment

    writer.saveWithMetadata(buffer, "/path/to/output/with_metadata.wav",
                           44100.0, 24, metadata);
}

//==============================================================================
// Example 6: Real-time recording to file
class AudioRecorder : public juce::AudioIODeviceCallback
{
public:
    AudioRecorder()
    {
        // Pre-allocate buffer for 10 seconds of stereo audio at 48kHz
        recordBuffer.setSize(2, 48000 * 10);
    }

    void startRecording()
    {
        recordPosition = 0;
        isRecording = true;
    }

    void stopRecording()
    {
        isRecording = false;
    }

    void audioDeviceIOCallback(const float** inputChannelData,
                              int numInputChannels,
                              float** outputChannelData,
                              int numOutputChannels,
                              int numSamples) override
    {
        if (isRecording && recordPosition + numSamples < recordBuffer.getNumSamples())
        {
            for (int channel = 0; channel < juce::jmin(numInputChannels, recordBuffer.getNumChannels()); ++channel)
            {
                recordBuffer.copyFrom(channel, recordPosition,
                                     inputChannelData[channel], numSamples);
            }
            recordPosition += numSamples;
        }

        // Clear output
        for (int channel = 0; channel < numOutputChannels; ++channel)
            if (outputChannelData[channel] != nullptr)
                juce::FloatVectorOperations::clear(outputChannelData[channel], numSamples);
    }

    void audioDeviceAboutToStart(juce::AudioIODevice* device) override
    {
        sampleRate = device->getCurrentSampleRate();
    }

    void audioDeviceStopped() override {}

    void saveRecording(const juce::String& filePath)
    {
        if (recordPosition > 0)
        {
            // Create a buffer with only the recorded samples
            juce::AudioBuffer<float> trimmedBuffer(recordBuffer.getNumChannels(), recordPosition);
            for (int channel = 0; channel < recordBuffer.getNumChannels(); ++channel)
            {
                trimmedBuffer.copyFrom(channel, 0, recordBuffer, channel, 0, recordPosition);
            }

            WavWriter writer;
            writer.saveToWavFile(trimmedBuffer, filePath, sampleRate, 24);
        }
    }

private:
    juce::AudioBuffer<float> recordBuffer;
    int recordPosition = 0;
    bool isRecording = false;
    double sampleRate = 44100.0;
};

//==============================================================================
// Example 7: Different bit depths
void exampleBitDepthComparison()
{
    WavWriter writer;
    juce::AudioBuffer<float> buffer(2, 88200);

    // ... fill buffer with audio data ...

    // Save with different bit depths
    writer.saveWithBitDepthConversion(buffer, "/path/to/output/16bit.wav", 44100.0, 16);
    writer.saveWithBitDepthConversion(buffer, "/path/to/output/24bit.wav", 44100.0, 24);

    // 16-bit: ~96 dB dynamic range, smaller file size
    // 24-bit: ~144 dB dynamic range, larger file size, better for professional audio
}
