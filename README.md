# JUCE Multi-Output Synthesizer and Audio Plugins

A collection of JUCE-based audio plugins demonstrating various audio processing techniques and multi-channel routing.

## Plugins Included

### 1. MultiOutSynth
A multi-output synthesizer plugin with 16 independent stereo outputs. Each MIDI channel routes to its own output bus, allowing for flexible mixing and processing in your DAW.

**Features:**
- 16 stereo output buses
- MIDI channel-based routing
- Sampler-based synthesis
- Up to 5 voices per MIDI channel

### 2. SurroundProcessor
A surround sound processor with level metering and bus visualization.

**Features:**
- Multi-bus input/output support
- Real-time level metering
- Interactive channel testing
- Flexible bus configuration

### 3. NoiseGate
A sidechain-enabled noise gate for dynamic audio control.

**Features:**
- Sidechain input
- Adjustable threshold and alpha parameters
- Low-pass filtering on sidechain signal

### 4. MidiLogger
A MIDI event logging and visualization plugin.

**Features:**
- Real-time MIDI message capture
- Tabular display of MIDI events
- Message history (stores up to 1000 messages)
- Clear button to reset log

### 5. ReaperEmbedded
A demonstration of REAPER-specific embedded UI features with level metering.

**Features:**
- REAPER embedded UI support
- VST2/VST3 extensions
- Real-time level visualization
- Global bypass integration

## Building

### Requirements
- CMake 3.15 or higher
- C++17 compatible compiler
- Git

### Build Instructions

```bash
# Configure the project
cmake -B Builds -DCMAKE_BUILD_TYPE=Release

# Build all plugins
cmake --build Builds --config Release

# Or build a specific plugin
cmake --build Builds --config Release --target MultiOutSynth
```

### Output

Built plugins will be located in:
- `Builds/Source/<PluginName>/<PluginName>_artefacts/`

Plugin formats:
- VST3
- Standalone applications

## Project Structure

```
.
├── CMakeLists.txt              # Main CMake configuration
├── Source/
│   ├── Assets/
│   │   └── DemoUtilities.h     # Shared utilities
│   ├── MultiOutSynth/          # Multi-output synthesizer
│   ├── SurroundProcessor/      # Surround sound processor
│   ├── NoiseGate/              # Noise gate with sidechain
│   ├── MidiLogger/             # MIDI event logger
│   └── ReaperEmbedded/         # REAPER embedded view demo
└── Builds/                     # Build output directory
```

## Usage

After building, load the plugins in your DAW of choice. Each plugin provides different functionality:

- **MultiOutSynth**: Load on a MIDI track and route different MIDI channels to different mixer channels for independent processing
- **SurroundProcessor**: Use for surround sound monitoring and channel testing
- **NoiseGate**: Insert on audio tracks with sidechain support
- **MidiLogger**: Insert on MIDI tracks to monitor MIDI events
- **ReaperEmbedded**: Special features when used in REAPER DAW

## License

MIT License - See LICENSE file for details

## Acknowledgments

Based on JUCE framework examples and demonstrations.
Built with [JUCE](https://juce.com/) 7.0.12
