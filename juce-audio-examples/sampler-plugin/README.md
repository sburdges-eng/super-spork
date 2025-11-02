# JUCE Sampler Plugin Example

Official JUCE example demonstrating a professional-grade sampler audio plugin with MPE (MIDI Polyphonic Expression) support.

## Overview

This is the complete source code for JUCE's SamplerPlugin example, showcasing:
- Professional audio plugin architecture
- Sample playback with pitch-shifting
- Loop point editing and visualization
- MPE and legacy MIDI support
- Waveform display with playback position
- Undo/redo functionality

## Features

### Audio Processing
- **Multi-voice synthesis** - Up to 16 simultaneous voices
- **Pitch-shifting** - Real-time sample playback at different pitches
- **Loop modes**:
  - None - One-shot playback
  - Forward - Continuous forward looping
  - Ping-pong - Bi-directional looping
- **Voice stealing** - Intelligent voice allocation

### MIDI Support
- **MPE (MIDI Polyphonic Expression)**:
  - Upper and lower zones
  - Per-note pitch bend
  - Per-note expression
  - Configurable zone layouts
- **Legacy MIDI**:
  - Standard MIDI channel mode
  - Configurable channel range
  - Pitch bend support

### User Interface
- **Waveform Display** - Visual representation of loaded sample
- **Loop Point Editor** - Interactive loop point markers
- **Playback Position** - Real-time position indicator
- **MPE Settings** - Configure MPE zones and parameters
- **Zoom/Pan** - Navigate through long samples

## Architecture

### Key Classes

#### Audio Classes
- `Sample` - Manages audio data storage
- `MPESamplerSound` - Extends juce::SynthesiserSound with loop and sample data
- `MPESamplerVoice` - Handles voice rendering and pitch-shifting
- `SamplerAudioProcessor` - Main plugin processor

#### Data Models
- `DataModel` - Manages sample, frequency, and loop settings
- `MPESettingsDataModel` - Handles MPE configuration
- `VisibleRangeDataModel` - Controls waveform zoom/pan

#### UI Components
- `MainSamplerView` - Main editor component
- `WaveformEditor` - Sample visualization
- `MPESettingsComponent` - MPE configuration interface
- `LoopPointsOverlay` - Interactive loop point editing

#### Thread Communication
- `CommandFifo` - Lock-free command queue
- `Command` - Type-erased command pattern
- Thread-safe GUI-to-audio communication

## Usage

### Building the Plugin

This is a JUCE PIP (Projucer Instant Project) that can be opened directly in Projucer:

1. Open Projucer
2. Select "Open Example Project"
3. Navigate to `SamplerPluginDemo.h`
4. Generate project for your IDE (Xcode, Visual Studio, etc.)
5. Build the plugin

### Using in a DAW

1. Build and install the plugin (VST3/AU)
2. Load in your DAW
3. Drag and drop an audio file onto the waveform display
4. Configure MPE settings if needed
5. Play MIDI notes to trigger the sample

### MIDI Mapping

The sample's root frequency can be configured. By default:
- MIDI note matching the root frequency plays at original pitch
- Higher notes pitch-shift upward
- Lower notes pitch-shift downward

### Loop Points

1. Enable looping in the interface
2. Drag loop markers on the waveform
3. Choose loop mode (forward/ping-pong)
4. Loop points are stored with the plugin state

## Code Highlights

### Sample Loading
```cpp
// Load audio file into sample
auto* reader = formatManager.createReaderFor(file);
if (reader != nullptr)
{
    auto sample = std::make_shared<Sample>(*reader, maxSampleLength);
    dataModel.setSampleReader(sample);
}
```

### Voice Rendering
```cpp
void MPESamplerVoice::renderNextBlock(AudioBuffer<float>& outputBuffer,
                                      int startSample, int numSamples)
{
    // Get sample data
    auto& sample = sound->getSample();

    // Apply pitch-shifting
    auto pitchRatio = std::pow(2.0, (currentlyPlayingNote.initialNote - rootNote) / 12.0);

    // Render with looping support
    // ... (see full implementation in source)
}
```

### Thread-Safe Commands
```cpp
// From GUI thread
commandFifo.push([this, newLoopMode]()
{
    // Executed on audio thread
    currentSound->setLoopMode(newLoopMode);
});
```

## Dependencies

### JUCE Modules Required
- `juce_audio_basics`
- `juce_audio_devices`
- `juce_audio_formats`
- `juce_audio_plugin_client`
- `juce_audio_processors`
- `juce_audio_utils`
- `juce_core`
- `juce_data_structures`
- `juce_events`
- `juce_graphics`
- `juce_gui_basics`
- `juce_gui_extra`

### C++ Requirements
- C++14 or later
- Standard library support

## Plugin Characteristics

```
Type: Audio Synthesizer
MIDI Input: Yes
Audio Output: Yes
MPE Support: Yes
Preset Save/Load: Yes
```

## Advanced Features

### ValueTree State Management
All plugin state is stored in a ValueTree structure:
- Sample data and parameters
- MPE settings
- Loop points and mode
- UI state (zoom, visible range)

### Undo/Redo
Uses JUCE's `UndoManager` for state changes:
- Parameter changes
- Loop point edits
- Sample loading

### Performance Optimizations
- Lock-free command queue for thread safety
- Efficient sample interpolation
- Voice stealing for polyphony management
- Memory-mapped file support

## Learning Points

This example demonstrates:

1. **Plugin Architecture** - Proper separation of audio/GUI threads
2. **MPE Implementation** - Complete MPE zone handling
3. **State Management** - ValueTree-based state system
4. **UI Design** - Custom waveform visualization
5. **Thread Safety** - Lock-free communication patterns
6. **Memory Management** - Smart pointer usage
7. **Audio Processing** - Sample playback and pitch-shifting

## Extending the Example

### Add Effects
```cpp
class MPESamplerVoiceWithEffects : public MPESamplerVoice
{
    void renderNextBlock(AudioBuffer<float>& outputBuffer, ...) override
    {
        // Render base sample
        MPESamplerVoice::renderNextBlock(outputBuffer, ...);

        // Apply effects
        filter.process(outputBuffer);
        reverb.process(outputBuffer);
    }

private:
    juce::IIRFilter filter;
    juce::Reverb reverb;
};
```

### Add ADSR Envelope
```cpp
class MPESamplerVoiceWithADSR : public MPESamplerVoice
{
    void renderNextBlock(AudioBuffer<float>& outputBuffer, ...) override
    {
        MPESamplerVoice::renderNextBlock(outputBuffer, ...);

        // Apply ADSR envelope
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float envelopeValue = adsr.getNextSample();
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
                outputBuffer.setSample(channel, sample,
                    outputBuffer.getSample(channel, sample) * envelopeValue);
        }
    }

private:
    juce::ADSR adsr;
};
```

### Multi-sample Support
Extend to load multiple samples mapped to different velocity or note ranges.

## Resources

- [JUCE Synthesiser Tutorial](https://docs.juce.com/master/tutorial_synth_using_midi_input.html)
- [MPE Documentation](https://docs.juce.com/master/classMPEInstrument.html)
- [JUCE Plugin Development](https://docs.juce.com/master/tutorial_create_projucer_basic_plugin.html)

## License

This code is part of the JUCE framework examples.
Copyright (c) Raw Material Software Limited

Licensed under the ISC license. See file header for full license text.
