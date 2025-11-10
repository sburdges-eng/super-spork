/*
  ==============================================================================

    Main.cpp
    Entry point for the JUCE Audio Player application

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
/**
 * This is the main application class that JUCE uses to start your program.
 * Think of this as the "power button" that turns on your app.
 */
class AudioPlayerApplication : public juce::JUCEApplication
{
public:
    //==============================================================================
    AudioPlayerApplication() {}

    /**
     * Returns the name of your application
     */
    const juce::String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }

    /**
     * Returns the version number
     */
    const juce::String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }

    /**
     * Can have multiple instances running at once?
     */
    bool moreThanOneInstanceAllowed() override
    {
        return true;
    }

    //==============================================================================
    /**
     * This is called when the application starts.
     * This is where we create the main window.
     */
    void initialise (const juce::String& commandLine) override
    {
        // Ignore command line for now
        juce::ignoreUnused (commandLine);

        // Create the main window
        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    /**
     * This is called when the application is being shut down.
     * Clean up and free resources here.
     */
    void shutdown() override
    {
        // Delete the main window
        mainWindow = nullptr;
    }

    //==============================================================================
    /**
     * Called when the system asks the app to quit (Cmd+Q or clicking X)
     */
    void systemRequestedQuit() override
    {
        // Close the app gracefully
        quit();
    }

    /**
     * Called when someone tries to re-launch the app while it's running
     */
    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // Not used in this simple app
        juce::ignoreUnused (commandLine);
    }

    //==============================================================================
    /**
     * This class represents the main window of the application.
     * It's a DocumentWindow, which is a window with a title bar and close button.
     */
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                            juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                        .findColour (juce::ResizableWindow::backgroundColourId),
                            DocumentWindow::allButtons)
        {
            // Make the window use native title bar (looks like other apps on your OS)
            setUsingNativeTitleBar (true);

            // Create and set the content component (this is where all the UI lives)
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            // On mobile, make it fullscreen
            setFullScreen (true);
           #else
            // On desktop, make it resizable and set initial size
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            // Make the window visible
            setVisible (true);
        }

        /**
         * Called when user clicks the close button (X)
         */
        void closeButtonPressed() override
        {
            // Tell the app to quit
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    // The main window (stored as a unique_ptr so it auto-deletes)
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
/**
 * This macro generates the main() function.
 * This is the actual entry point of the program when you run it.
 * It creates an instance of AudioPlayerApplication and runs it.
 */
START_JUCE_APPLICATION (AudioPlayerApplication)
