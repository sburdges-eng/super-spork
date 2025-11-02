/*
  ==============================================================================

   Simple Audio Player - Usage Examples

  ==============================================================================
*/

#include "SimpleAudioPlayer.h"

//==============================================================================
// Example 1: Basic playback
void exampleBasicPlayback()
{
    SimpleAudioPlayer player;

    if (player.loadFile("/path/to/your/audio.wav"))
    {
        player.play();

        // Wait for playback to finish or stop manually
        juce::Thread::sleep(5000);
        player.stop();
    }
}

//==============================================================================
// Example 2: Player with audio device
class AudioPlayerComponent : public juce::Component
{
public:
    AudioPlayerComponent()
    {
        // Set up audio device
        audioDeviceManager.initialiseWithDefaultDevices(0, 2);
        audioDeviceManager.addAudioCallback(&audioSourcePlayer);
        audioSourcePlayer.setSource(&player);

        // Set up callback for when playback finishes
        player.onPlaybackFinished = [this]()
        {
            DBG("Playback finished!");
            // Could restart, load next file, etc.
        };
    }

    ~AudioPlayerComponent() override
    {
        audioSourcePlayer.setSource(nullptr);
        audioDeviceManager.removeAudioCallback(&audioSourcePlayer);
    }

    void loadAndPlay(const juce::String& filePath)
    {
        if (player.loadFile(filePath))
        {
            DBG("Loaded: " + filePath);
            DBG("Duration: " + juce::String(player.getLengthInSeconds()) + " seconds");
            player.play();
        }
    }

    void stop()
    {
        player.stop();
    }

    void setVolume(float volume)
    {
        player.setGain(volume); // 0.0 to 1.0
    }

private:
    SimpleAudioPlayer player;
    juce::AudioDeviceManager audioDeviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;
};

//==============================================================================
// Example 3: Looping player
void exampleLoopingPlayback()
{
    SimpleAudioPlayer player;

    if (player.loadFile("/path/to/loop.wav"))
    {
        player.setLooping(true);
        player.play();

        // Will loop forever until stopped
        juce::Thread::sleep(10000);
        player.stop();
    }
}

//==============================================================================
// Example 4: Playlist player
class PlaylistPlayer
{
public:
    PlaylistPlayer()
    {
        player.onPlaybackFinished = [this]()
        {
            playNext();
        };

        audioDeviceManager.initialiseWithDefaultDevices(0, 2);
        audioDeviceManager.addAudioCallback(&audioSourcePlayer);
        audioSourcePlayer.setSource(&player);
    }

    ~PlaylistPlayer()
    {
        audioSourcePlayer.setSource(nullptr);
        audioDeviceManager.removeAudioCallback(&audioSourcePlayer);
    }

    void addToPlaylist(const juce::String& filePath)
    {
        playlist.add(juce::File(filePath));
    }

    void play()
    {
        if (playlist.size() > 0)
        {
            currentIndex = 0;
            playCurrentTrack();
        }
    }

    void playNext()
    {
        if (currentIndex < playlist.size() - 1)
        {
            currentIndex++;
            playCurrentTrack();
        }
        else
        {
            DBG("Playlist finished");
        }
    }

    void playPrevious()
    {
        if (currentIndex > 0)
        {
            currentIndex--;
            playCurrentTrack();
        }
    }

    void stop()
    {
        player.stop();
    }

private:
    void playCurrentTrack()
    {
        if (currentIndex >= 0 && currentIndex < playlist.size())
        {
            if (player.loadFile(playlist[currentIndex]))
            {
                DBG("Now playing: " + playlist[currentIndex].getFileName());
                player.play();
            }
        }
    }

    SimpleAudioPlayer player;
    juce::AudioDeviceManager audioDeviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;
    juce::Array<juce::File> playlist;
    int currentIndex = -1;
};

//==============================================================================
// Example 5: Player with position control
void examplePositionControl()
{
    SimpleAudioPlayer player;

    if (player.loadFile("/path/to/audio.wav"))
    {
        // Skip to 5 seconds
        player.setPosition(5.0);
        player.play();

        // Play for 3 seconds
        juce::Thread::sleep(3000);

        // Jump to 50% through the file
        double halfwayPoint = player.getLengthInSeconds() * 0.5;
        player.setPosition(halfwayPoint);

        // Continue playing
        juce::Thread::sleep(3000);
        player.stop();
    }
}

//==============================================================================
// Example 6: Fade in/out
class FadingAudioPlayer
{
public:
    void loadFile(const juce::String& filePath)
    {
        player.loadFile(filePath);
    }

    void playWithFadeIn(float fadeTimeSeconds = 2.0f)
    {
        player.setGain(0.0f);
        player.play();

        // Gradually increase volume
        const int steps = 100;
        const int sleepTime = (int)(fadeTimeSeconds * 1000.0f / steps);

        for (int i = 0; i <= steps; ++i)
        {
            player.setGain((float)i / steps);
            juce::Thread::sleep(sleepTime);
        }
    }

    void stopWithFadeOut(float fadeTimeSeconds = 2.0f)
    {
        // Gradually decrease volume
        const int steps = 100;
        const int sleepTime = (int)(fadeTimeSeconds * 1000.0f / steps);

        for (int i = steps; i >= 0; --i)
        {
            player.setGain((float)i / steps);
            juce::Thread::sleep(sleepTime);
        }

        player.stop();
        player.setGain(1.0f); // Reset for next playback
    }

private:
    SimpleAudioPlayer player;
};
