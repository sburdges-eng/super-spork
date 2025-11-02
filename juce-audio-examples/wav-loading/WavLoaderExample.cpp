/*
  ==============================================================================

   WAV File Loading Example - Usage Demo

  ==============================================================================
*/

#include "WavLoader.h"

//==============================================================================
// Example 1: Basic WAV file loading
void exampleBasicLoading()
{
    WavLoader loader;
    juce::AudioBuffer<float> audioBuffer;

    // Load a WAV file
    if (loader.loadWavFile("/path/to/your/audio.wav", audioBuffer))
    {
        // Successfully loaded! Now you can use the audio buffer
        int numChannels = audioBuffer.getNumChannels();
        int numSamples = audioBuffer.getNumSamples();

        // Access audio data
        float* leftChannel = audioBuffer.getWritePointer(0);
        float* rightChannel = numChannels > 1 ? audioBuffer.getWritePointer(1) : nullptr;

        // Process audio...
    }
}

//==============================================================================
// Example 2: Load a specific section of a WAV file
void exampleSectionLoading()
{
    WavLoader loader;
    juce::AudioBuffer<float> audioBuffer;

    // Load 44100 samples (1 second at 44.1kHz) starting from sample 22050
    if (loader.loadWavFileSection("/path/to/your/audio.wav", audioBuffer, 22050, 44100))
    {
        // Buffer now contains 1 second of audio starting at 0.5 seconds
    }
}

//==============================================================================
// Example 3: Get file information without loading
void exampleGetFileInfo()
{
    WavLoader loader;
    WavLoader::AudioFileInfo info;

    if (loader.getAudioFileInfo("/path/to/your/audio.wav", info))
    {
        DBG("Sample Rate: " + juce::String(info.sampleRate));
        DBG("Channels: " + juce::String(info.numChannels));
        DBG("Duration: " + juce::String(info.durationSeconds) + " seconds");
        DBG("Bits per sample: " + juce::String(info.bitsPerSample));
    }
}

//==============================================================================
// Example 4: Load and process audio in chunks (for large files)
void exampleChunkedLoading()
{
    WavLoader loader;
    juce::AudioBuffer<float> chunkBuffer;

    const int chunkSize = 44100; // 1 second chunks at 44.1kHz
    int64 currentPosition = 0;

    // Load first chunk to get file info
    if (loader.loadWavFileSection("/path/to/your/audio.wav", chunkBuffer, 0, chunkSize))
    {
        // Process first chunk...
        currentPosition += chunkSize;

        // Continue loading chunks until end of file
        while (loader.loadWavFileSection("/path/to/your/audio.wav",
                                         chunkBuffer,
                                         currentPosition,
                                         chunkSize))
        {
            // Process each chunk...
            currentPosition += chunkSize;
        }
    }
}

//==============================================================================
// Example 5: Load multiple files into an array
void exampleLoadMultipleFiles()
{
    WavLoader loader;
    std::vector<juce::AudioBuffer<float>> samples;

    juce::StringArray filePaths = {
        "/path/to/kick.wav",
        "/path/to/snare.wav",
        "/path/to/hihat.wav"
    };

    for (const auto& path : filePaths)
    {
        juce::AudioBuffer<float> buffer;
        if (loader.loadWavFile(path, buffer))
        {
            samples.push_back(std::move(buffer));
        }
    }

    // Now you have all samples loaded in the samples vector
    DBG("Loaded " + juce::String(samples.size()) + " samples");
}
