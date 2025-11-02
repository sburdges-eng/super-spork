/*
  ==============================================================================

   Simple Audio Player - JUCE
   A minimal example of an audio player that can load and play WAV files

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SimpleAudioPlayer : public juce::AudioSource,
                          public juce::ChangeListener
{
public:
    SimpleAudioPlayer()
    {
        formatManager.registerBasicFormats();
        transportSource.addChangeListener(this);
    }

    ~SimpleAudioPlayer() override
    {
        transportSource.setSource(nullptr);
    }

    //==============================================================================
    /**
     * Load an audio file
     */
    bool loadFile(const juce::File& audioFile)
    {
        if (!audioFile.existsAsFile())
            return false;

        auto* reader = formatManager.createReaderFor(audioFile);

        if (reader != nullptr)
        {
            std::unique_ptr<juce::AudioFormatReaderSource> newSource(
                new juce::AudioFormatReaderSource(reader, true));

            transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            readerSource.reset(newSource.release());

            currentFile = audioFile;
            return true;
        }

        return false;
    }

    /**
     * Load from file path
     */
    bool loadFile(const juce::String& filePath)
    {
        return loadFile(juce::File(filePath));
    }

    //==============================================================================
    // Playback controls
    void play()
    {
        transportSource.start();
    }

    void stop()
    {
        transportSource.stop();
    }

    void pause()
    {
        transportSource.stop();
    }

    bool isPlaying() const
    {
        return transportSource.isPlaying();
    }

    //==============================================================================
    // Position controls
    void setPosition(double positionSeconds)
    {
        transportSource.setPosition(positionSeconds);
    }

    double getPosition() const
    {
        return transportSource.getCurrentPosition();
    }

    double getLengthInSeconds() const
    {
        return transportSource.getLengthInSeconds();
    }

    //==============================================================================
    // Volume control
    void setGain(float newGain)
    {
        transportSource.setGain(newGain);
    }

    float getGain() const
    {
        return transportSource.getGain();
    }

    //==============================================================================
    // Loop control
    void setLooping(bool shouldLoop)
    {
        if (readerSource != nullptr)
            readerSource->setLooping(shouldLoop);
    }

    //==============================================================================
    // AudioSource interface
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }

        transportSource.getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override
    {
        transportSource.releaseResources();
    }

    //==============================================================================
    // ChangeListener interface
    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource)
        {
            if (transportSource.hasStreamFinished())
            {
                // File has finished playing
                onPlaybackFinished();
            }
        }
    }

    //==============================================================================
    // Callback when playback finishes
    std::function<void()> onPlaybackFinished = []() {};

    //==============================================================================
    juce::File getCurrentFile() const { return currentFile; }

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::File currentFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleAudioPlayer)
};
