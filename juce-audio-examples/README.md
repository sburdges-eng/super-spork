# JUCE Audio Sample Examples

A comprehensive collection of code examples demonstrating how to work with audio samples in JUCE, including loading, saving, playing, and processing WAV files.

## Overview

This repository contains practical, production-ready examples for:
- Loading WAV files into AudioBuffer
- Saving AudioBuffer to WAV files
- Playing audio samples
- Building samplers and drum machines
- Audio processing utilities

## Contents

### 📁 [sampler-plugin](./sampler-plugin/)
Official JUCE SamplerPlugin example with MPE support
- Full-featured sampler plugin implementation
- Loop mode support (none, forward, ping-pong)
- Waveform visualization
- MPE (MIDI Polyphonic Expression) support
- Professional plugin architecture

### 📁 [wav-loading](./wav-loading/)
Complete examples for loading WAV files
- Basic WAV file loading
- Loading specific sections of files
- Chunked loading for large files
- Loading multiple files
- Getting file information without loading

### 📁 [wav-saving](./wav-saving/)
Complete examples for saving audio to WAV files
- Basic WAV file writing
- Different bit depths (16, 24, 32)
- Saving with metadata
- Separate channel export
- Real-time recording to file

### 📁 [utils](./utils/)
Utility classes and helper functions
- `SimpleAudioPlayer` - Easy-to-use audio player
- `SimpleSampler` - MIDI-triggered sample playback
- `SimpleDrumMachine` - Drum kit sampler
- `AudioBufferUtilities` - Buffer processing functions
- `SampleLibrary` - Sample management system

## Quick Start

### Loading a WAV File

```cpp
#include "wav-loading/WavLoader.h"

WavLoader loader;
juce::AudioBuffer<float> audioBuffer;

if (loader.loadWavFile("/path/to/audio.wav", audioBuffer))
{
    // Successfully loaded!
    int numChannels = audioBuffer.getNumChannels();
    int numSamples = audioBuffer.getNumSamples();
    // Process audio...
}
```

### Saving a WAV File

```cpp
#include "wav-saving/WavWriter.h"

WavWriter writer;
juce::AudioBuffer<float> buffer(2, 88200); // 2 channels, 2 seconds @ 44.1kHz

// Fill buffer with audio data...

writer.saveToWavFile(buffer, "/path/to/output.wav", 44100.0, 24);
```

### Playing Audio

```cpp
#include "utils/SimpleAudioPlayer.h"

SimpleAudioPlayer player;

if (player.loadFile("/path/to/audio.wav"))
{
    player.play();
}
```

### Building a Simple Sampler

```cpp
#include "utils/SamplePlayerUtilities.h"

SimpleSampler sampler;

// Load samples mapped to MIDI notes
sampler.loadSample("/path/to/C3.wav", 60);  // C3
sampler.loadSample("/path/to/E3.wav", 64);  // E3
sampler.loadSample("/path/to/G3.wav", 67);  // G3

// Trigger via MIDI
sampler.noteOn(1, 60, 100);  // Play C3
```

## Requirements

- JUCE Framework 7.0 or later
- C++17 or later
- Supported platforms: macOS, Windows, Linux

## Integration

### Option 1: Copy Files
Simply copy the header files you need into your JUCE project.

### Option 2: Add as Subdirectory
Add this as a subdirectory in your project and include the files you need.

### JUCE Modules Required
Make sure your project includes these JUCE modules:
- `juce_audio_basics`
- `juce_audio_formats`
- `juce_audio_processors`
- `juce_audio_devices`
- `juce_core`

## Examples Overview

### WAV Loading Examples
1. **Basic Loading** - Load entire WAV file into buffer
2. **Section Loading** - Load specific portions of large files
3. **File Info** - Get metadata without loading
4. **Chunked Loading** - Process large files in chunks
5. **Multiple Files** - Load multiple samples efficiently

### WAV Saving Examples
1. **Basic Writing** - Save buffer to WAV file
2. **Section Writing** - Save specific portions
3. **Metadata** - Add custom metadata to files
4. **Bit Depth Conversion** - Save in different bit depths
5. **Channel Separation** - Export channels as separate files
6. **Real-time Recording** - Record audio input to file

### Audio Player Examples
1. **Basic Playback** - Simple play/stop functionality
2. **Looping** - Continuous loop playback
3. **Position Control** - Seek to specific positions
4. **Playlist** - Sequential playback of multiple files
5. **Fade In/Out** - Volume automation
6. **Volume Control** - Real-time gain adjustment

### Sampler Examples
1. **Simple Sampler** - MIDI-triggered samples
2. **Drum Machine** - Pre-mapped drum kit
3. **Sample Library** - Organized sample management
4. **Multi-sample** - Multiple samples per instrument

## Audio Processing Utilities

The `AudioBufferUtilities` class provides:
- **Normalization** - Adjust peak levels
- **Fade In/Out** - Apply fade curves
- **Reverse** - Reverse audio playback
- **Mix** - Combine multiple buffers
- **Mono Conversion** - Convert stereo to mono
- **Resampling** - Change sample rate

Example:
```cpp
// Normalize audio to -3dB peak
AudioBufferUtilities::normalize(buffer, 0.707f);

// Apply 1-second fade in (at 44.1kHz)
AudioBufferUtilities::fadeIn(buffer, 44100);

// Reverse the audio
AudioBufferUtilities::reverse(buffer);
```

## Advanced Features

### Real-time Audio Recording
See `WavWriterExample.cpp` for a complete AudioRecorder class that can:
- Record from audio input
- Save to WAV file
- Handle buffer management
- Support multiple channels

### Sample Library Management
The `SampleLibrary` class provides:
- Organized sample storage
- Name-based sample retrieval
- Automatic format detection
- Memory-efficient loading

### Professional Sampler Plugin
The official JUCE SamplerPlugin example includes:
- MPE support for expressive control
- Loop point editing
- Waveform visualization
- Multiple playback modes
- Undo/redo functionality

## Best Practices

### Loading Audio Files
1. Always check if file exists before loading
2. Use `AudioFormatManager` for format detection
3. Load large files on background thread
4. Free resources when done

### Saving Audio Files
1. Delete existing file before writing
2. Create parent directories if needed
3. Use appropriate bit depth (24-bit recommended)
4. Flush writer before closing

### Performance
1. Pre-allocate buffers when possible
2. Use memory-mapped files for large samples
3. Load samples asynchronously
4. Cache frequently used samples

### Memory Management
1. Use `std::unique_ptr` for readers/writers
2. Clear unused buffers promptly
3. Monitor memory usage for large sample sets
4. Consider streaming for very long files

## Troubleshooting

### File Won't Load
- Check file exists and path is correct
- Ensure audio format is supported (WAV, AIFF, etc.)
- Verify file is not corrupted
- Check file permissions

### Playback Issues
- Ensure audio device is initialized
- Check sample rate compatibility
- Verify buffer sizes are appropriate
- Monitor CPU usage

### Memory Issues
- Use chunked loading for large files
- Clear unused samples
- Consider streaming instead of buffering
- Profile memory usage

## Resources

### Official JUCE Documentation
- [Build an audio player](https://docs.juce.com/master/tutorial_playing_sound_files.html)
- [AudioFormatReader](https://docs.juce.com/master/classAudioFormatReader.html)
- [AudioFormatWriter](https://docs.juce.com/master/classAudioFormatWriter.html)
- [AudioBuffer](https://docs.juce.com/master/classAudioBuffer.html)

### JUCE Forum
- [forum.juce.com](https://forum.juce.com/)

### GitHub Examples
- [JUCE Examples](https://github.com/juce-framework/JUCE/tree/master/examples)
- [Awesome JUCE](https://github.com/sudara/awesome-juce)

## License

These examples are provided as educational resources. The official JUCE SamplerPlugin example is licensed under the ISC license (see file header). Other examples can be freely used in your projects.

## Contributing

Feel free to submit issues or pull requests with improvements, bug fixes, or additional examples.

## Credits

- Official SamplerPlugin example from [JUCE Framework](https://github.com/juce-framework/JUCE)
- Community examples and best practices from JUCE Forum
- Additional examples created for educational purposes
