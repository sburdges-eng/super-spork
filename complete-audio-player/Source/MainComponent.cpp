/*
  ==============================================================================

    MainComponent.cpp
    Implementation of the main user interface

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Register audio formats (WAV, AIFF, FLAC, MP3, etc.)
    formatManager.registerBasicFormats();

    // Setup audio device (2 output channels for stereo)
    audioDeviceManager.initialiseWithDefaultDevices (0, 2);
    audioDeviceManager.addAudioCallback (&audioSourcePlayer);
    audioSourcePlayer.setSource (&transportSource);

    //==============================================================================
    // Setup LOAD button
    addAndMakeVisible (loadButton);
    loadButton.setButtonText ("Load Audio File");
    loadButton.addListener (this);
    loadButton.setColour (juce::TextButton::buttonColourId, juce::Colours::darkblue);

    // Setup PLAY button
    addAndMakeVisible (playButton);
    playButton.setButtonText ("Play");
    playButton.addListener (this);
    playButton.setEnabled (false); // Disabled until file is loaded
    playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);

    // Setup STOP button
    addAndMakeVisible (stopButton);
    stopButton.setButtonText ("Stop");
    stopButton.addListener (this);
    stopButton.setEnabled (false);
    stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);

    // Setup SAVE button
    addAndMakeVisible (saveButton);
    saveButton.setButtonText ("Save Copy As...");
    saveButton.addListener (this);
    saveButton.setEnabled (false);
    saveButton.setColour (juce::TextButton::buttonColourId, juce::Colours::orange);

    //==============================================================================
    // Setup VOLUME slider
    addAndMakeVisible (volumeSlider);
    volumeSlider.setRange (0.0, 1.0);
    volumeSlider.setValue (0.7);
    volumeSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    volumeSlider.addListener (this);
    volumeSlider.setColour (juce::Slider::thumbColourId, juce::Colours::green);

    addAndMakeVisible (volumeLabel);
    volumeLabel.setText ("Volume:", juce::dontSendNotification);
    volumeLabel.attachToComponent (&volumeSlider, true);

    // Setup POSITION slider
    addAndMakeVisible (positionSlider);
    positionSlider.setRange (0.0, 1.0);
    positionSlider.setValue (0.0);
    positionSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    positionSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    positionSlider.addListener (this);
    positionSlider.setEnabled (false);
    positionSlider.setColour (juce::Slider::thumbColourId, juce::Colours::blue);

    addAndMakeVisible (positionLabel);
    positionLabel.setText ("Position:", juce::dontSendNotification);
    positionLabel.attachToComponent (&positionSlider, true);

    //==============================================================================
    // Setup info labels
    addAndMakeVisible (fileLabel);
    fileLabel.setText ("No file loaded", juce::dontSendNotification);
    fileLabel.setJustificationType (juce::Justification::centred);
    fileLabel.setColour (juce::Label::backgroundColourId, juce::Colours::darkgrey);
    fileLabel.setColour (juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible (statusLabel);
    statusLabel.setText ("Status: Stopped", juce::dontSendNotification);
    statusLabel.setJustificationType (juce::Justification::centred);
    statusLabel.setColour (juce::Label::backgroundColourId, juce::Colours::black);
    statusLabel.setColour (juce::Label::textColourId, juce::Colours::lightgreen);

    addAndMakeVisible (infoLabel);
    infoLabel.setText ("", juce::dontSendNotification);
    infoLabel.setJustificationType (juce::Justification::centred);
    infoLabel.setColour (juce::Label::backgroundColourId, juce::Colours::darkgrey.darker());
    infoLabel.setColour (juce::Label::textColourId, juce::Colours::white);

    //==============================================================================
    // Start timer to update position display (30 times per second)
    startTimer (30);

    // Set initial window size
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    // Stop the timer
    stopTimer();

    // Shutdown audio
    transportSource.setSource (nullptr);
    audioSourcePlayer.setSource (nullptr);
    audioDeviceManager.removeAudioCallback (&audioSourcePlayer);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // Fill background with a gradient
    g.fillAll (juce::Colours::darkslategrey);

    // Draw a border around the whole component
    g.setColour (juce::Colours::lightblue);
    g.drawRect (getLocalBounds(), 2);

    // Draw title
    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    g.drawText ("JUCE Audio Player", getLocalBounds().removeFromTop(40),
                juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // Layout all the UI components
    auto area = getLocalBounds();

    // Title area
    area.removeFromTop (50);

    // File info label
    fileLabel.setBounds (area.removeFromTop (30).reduced (10, 5));

    // Status label
    statusLabel.setBounds (area.removeFromTop (25).reduced (10, 2));

    // Info label (sample rate, channels, etc.)
    infoLabel.setBounds (area.removeFromTop (25).reduced (10, 2));

    area.removeFromTop (10); // Spacer

    // Buttons
    auto buttonArea = area.removeFromTop (50).reduced (10, 5);
    auto buttonWidth = buttonArea.getWidth() / 4;
    loadButton.setBounds (buttonArea.removeFromLeft (buttonWidth).reduced (5, 0));
    playButton.setBounds (buttonArea.removeFromLeft (buttonWidth).reduced (5, 0));
    stopButton.setBounds (buttonArea.removeFromLeft (buttonWidth).reduced (5, 0));
    saveButton.setBounds (buttonArea.removeFromLeft (buttonWidth).reduced (5, 0));

    area.removeFromTop (20); // Spacer

    // Volume slider
    auto volumeArea = area.removeFromTop (40).reduced (10, 5);
    volumeArea.removeFromLeft (80); // Space for label
    volumeSlider.setBounds (volumeArea);

    // Position slider
    auto positionArea = area.removeFromTop (40).reduced (10, 5);
    positionArea.removeFromLeft (80); // Space for label
    positionSlider.setBounds (positionArea);
}

//==============================================================================
void MainComponent::buttonClicked (juce::Button* button)
{
    if (button == &loadButton)
    {
        // Open file chooser to select an audio file
        fileChooser = std::make_unique<juce::FileChooser> (
            "Select an audio file to play...",
            juce::File{},
            "*.wav;*.mp3;*.aiff;*.flac"
        );

        auto chooserFlags = juce::FileBrowserComponent::openMode
                          | juce::FileBrowserComponent::canSelectFiles;

        fileChooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file != juce::File{})
                loadAudioFile (file);
        });
    }
    else if (button == &playButton)
    {
        // Start playback
        transportSource.start();
        statusLabel.setText ("Status: Playing", juce::dontSendNotification);
        playButton.setEnabled (false);
        stopButton.setEnabled (true);
    }
    else if (button == &stopButton)
    {
        // Stop playback
        transportSource.stop();
        statusLabel.setText ("Status: Stopped", juce::dontSendNotification);
        playButton.setEnabled (true);
        stopButton.setEnabled (false);
    }
    else if (button == &saveButton)
    {
        // Save a copy of the current file
        if (currentAudioBuffer.getNumSamples() > 0)
        {
            fileChooser = std::make_unique<juce::FileChooser> (
                "Save audio as...",
                juce::File{},
                "*.wav"
            );

            auto chooserFlags = juce::FileBrowserComponent::saveMode
                              | juce::FileBrowserComponent::canSelectFiles;

            fileChooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
            {
                auto file = fc.getResult();

                if (file != juce::File{})
                {
                    // Delete existing file
                    if (file.existsAsFile())
                        file.deleteFile();

                    // Create WAV writer
                    juce::WavAudioFormat wavFormat;
                    std::unique_ptr<juce::FileOutputStream> fileStream (file.createOutputStream());

                    if (fileStream != nullptr)
                    {
                        std::unique_ptr<juce::AudioFormatWriter> writer;
                        writer.reset (wavFormat.createWriterFor (fileStream.get(),
                                                                 currentSampleRate,
                                                                 currentAudioBuffer.getNumChannels(),
                                                                 24,  // 24-bit
                                                                 {},
                                                                 0));

                        if (writer != nullptr)
                        {
                            fileStream.release(); // Writer takes ownership
                            writer->writeFromAudioSampleBuffer (currentAudioBuffer, 0,
                                                                currentAudioBuffer.getNumSamples());

                            statusLabel.setText ("Status: File saved successfully!",
                                               juce::dontSendNotification);
                        }
                    }
                }
            });
        }
    }
}

void MainComponent::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        // Update volume
        transportSource.setGain ((float) volumeSlider.getValue());
    }
    else if (slider == &positionSlider)
    {
        // Update playback position (only when user drags it)
        if (! positionSlider.isMouseButtonDown())
            return;

        auto newPosition = positionSlider.getValue();
        transportSource.setPosition (newPosition);
    }
}

void MainComponent::timerCallback()
{
    // Update position display while playing
    if (transportSource.isPlaying())
    {
        updatePositionDisplay();

        // Check if finished
        if (transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds())
        {
            transportSource.stop();
            transportSource.setPosition (0.0);
            statusLabel.setText ("Status: Finished", juce::dontSendNotification);
            playButton.setEnabled (true);
            stopButton.setEnabled (false);
        }
    }
}

//==============================================================================
void MainComponent::loadAudioFile (const juce::File& file)
{
    // Stop current playback
    transportSource.stop();
    transportSource.setSource (nullptr);
    readerSource.reset();

    // Create reader for the file
    auto* reader = formatManager.createReaderFor (file);

    if (reader != nullptr)
    {
        // Store current file
        currentFile = file;
        currentSampleRate = reader->sampleRate;

        // Load into buffer for saving later
        currentAudioBuffer.setSize ((int) reader->numChannels,
                                    (int) reader->lengthInSamples);
        reader->read (&currentAudioBuffer, 0, (int) reader->lengthInSamples, 0, true, true);

        // Create audio source from reader
        auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset (newSource.release());

        // Update UI
        fileLabel.setText (file.getFileName(), juce::dontSendNotification);
        updateFileInfo();

        // Update position slider range
        positionSlider.setRange (0.0, transportSource.getLengthInSeconds());
        positionSlider.setValue (0.0, juce::dontSendNotification);
        positionSlider.setEnabled (true);

        // Enable buttons
        playButton.setEnabled (true);
        saveButton.setEnabled (true);

        statusLabel.setText ("Status: File loaded successfully", juce::dontSendNotification);
    }
    else
    {
        statusLabel.setText ("Status: Error loading file!", juce::dontSendNotification);
    }
}

void MainComponent::updatePositionDisplay()
{
    auto currentPos = transportSource.getCurrentPosition();
    positionSlider.setValue (currentPos, juce::dontSendNotification);
}

void MainComponent::updateFileInfo()
{
    if (currentAudioBuffer.getNumSamples() > 0)
    {
        auto duration = currentAudioBuffer.getNumSamples() / currentSampleRate;
        auto channels = currentAudioBuffer.getNumChannels();

        juce::String info;
        info << (int) currentSampleRate << " Hz | ";
        info << channels << (channels == 1 ? " Channel | " : " Channels | ");
        info << juce::String (duration, 2) << " seconds";

        infoLabel.setText (info, juce::dontSendNotification);
    }
}
