# JUCE Examples CMake Project

This repository contains JUCE framework examples built with CMake.

## Project Structure

```
.
├── CMakeLists.txt           # Root CMake configuration
├── examples/                # Examples directory
│   ├── CMakeLists.txt      # Examples CMake configuration
│   ├── Audio/              # Audio examples
│   ├── DSP/                # DSP examples
│   ├── GUI/                # GUI examples
│   ├── Plugins/            # Plugin examples
│   │   └── ArpeggiatorPlugin.h  # MIDI arpeggiator plugin
│   ├── Utilities/          # Utility examples
│   ├── CMake/              # CMake-specific examples
│   └── DemoRunner/         # DemoRunner application
```

## Building

### Prerequisites

- CMake 3.22 or later
- A C++17 compatible compiler
- Git (for fetching JUCE)

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure CMake
cmake ..

# Build all examples
cmake --build .

# Or build specific example
cmake --build . --target ArpeggiatorPlugin_VST3
```

## Adding New Examples

To add a new example, create a `.h` file in one of the example subdirectories (Audio, DSP, GUI, Plugins, Utilities) with proper PIP metadata:

```cpp
/*******************************************************************************
 BEGIN_JUCE_PIP_METADATA

 name:             YourPluginName
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Your plugin description.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_plugin_client, juce_audio_processors,
                   juce_audio_utils, juce_core, juce_data_structures,
                   juce_events, juce_graphics, juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2022, linux_make

 type:             AudioProcessor
 mainClass:        YourClassName

 END_JUCE_PIP_METADATA
*******************************************************************************/
```

The CMake configuration will automatically discover and build all `.h` files with PIP metadata.

## JUCE Version

This project uses JUCE 7.0.12, fetched automatically via CMake's FetchContent.

## License

The examples are provided under the ISC license. See individual files for details.
