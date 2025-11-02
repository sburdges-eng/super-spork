cmake_minimum_required(VERSION 3.15)

project(JUCE_MULTI_OUT_SYNTH_PLUGINS VERSION 1.0.0)

# Fetch JUCE framework
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories for each plugin
add_subdirectory(Source/MultiOutSynth)
add_subdirectory(Source/SurroundProcessor)
add_subdirectory(Source/ReaperEmbedded)
add_subdirectory(Source/NoiseGate)
add_subdirectory(Source/MidiLogger)
