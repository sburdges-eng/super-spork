#pragma once

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_basics/juce_audio_basics.h>

using namespace juce;

//==============================================================================
// Demo utilities for JUCE plugins

inline std::unique_ptr<InputStream> createAssetInputStream(const char* assetName)
{
    // Generate a simple sine wave as a fallback for the "singing.ogg" asset
    if (String(assetName) == "singing.ogg")
    {
        // Create a simple sine wave buffer
        constexpr int sampleRate = 44100;
        constexpr int numSeconds = 2;
        constexpr int numSamples = sampleRate * numSeconds;
        constexpr double frequency = 440.0; // A4 note

        AudioBuffer<float> buffer(1, numSamples);
        auto* data = buffer.getWritePointer(0);

        for (int i = 0; i < numSamples; ++i)
        {
            data[i] = static_cast<float>(std::sin(2.0 * MathConstants<double>::pi * frequency * i / sampleRate));
        }

        // Create a temporary WAV file in memory
        MemoryBlock memoryBlock;
        {
            WavAudioFormat wavFormat;
            std::unique_ptr<AudioFormatWriter> writer(
                wavFormat.createWriterFor(new MemoryOutputStream(memoryBlock, false),
                                         sampleRate,
                                         1,
                                         16,
                                         {},
                                         0));
            if (writer != nullptr)
                writer->writeFromAudioSampleBuffer(buffer, 0, numSamples);
        }

        return std::make_unique<MemoryInputStream>(memoryBlock, false);
    }

    return nullptr;
}
