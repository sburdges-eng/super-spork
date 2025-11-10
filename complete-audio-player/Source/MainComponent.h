/*
  ==============================================================================

    MainComponent.h
    The main user interface for the audio player

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * This is the main component that contains all the UI elements.
 * It inherits from:
 * - Component: Makes it a visual element
 * - Button::Listener: Allows it to respond to button clicks
 * - Slider::Listener: Allows it to respond to slider changes
 * - Timer: Allows it to update display periodically
 */
class MainComponent : public juce::Component,
                      public juce::Button::Listener,
                      public juce::Slider::Listener,
                      public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    // Component virtual functions we need to implement
    void paint (juce::Graphics&) override;
    void resized() override;

    // Button::Listener virtual function
    void buttonClicked (juce::Button*) override;

    // Slider::Listener virtual function
    void sliderValueChanged (juce::Slider* slider) override;

    // Timer callback
    void timerCallback() override;

private:
    //==============================================================================
    // Audio components
    juce::AudioDeviceManager audioDeviceManager;
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioSourcePlayer audioSourcePlayer;

    // Current file info
    juce::File currentFile;
    double currentSampleRate = 0.0;

    // UI Components - Buttons
    juce::TextButton loadButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton saveButton;

    // UI Components - Sliders
    juce::Slider volumeSlider;
    juce::Label volumeLabel;

    juce::Slider positionSlider;
    juce::Label positionLabel;

    // UI Components - Labels
    juce::Label fileLabel;
    juce::Label statusLabel;
    juce::Label infoLabel;

    // File chooser (for opening files)
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Current audio buffer (for saving)
    juce::AudioBuffer<float> currentAudioBuffer;

    // Helper functions
    void loadAudioFile (const juce::File& file);
    void updatePositionDisplay();
    void updateFileInfo();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
