# JUCE Audio Player - CMake Build Configuration
# This file tells CMake how to build the application

cmake_minimum_required(VERSION 3.15)

# Project name and version
project(AudioPlayer VERSION 1.0.0)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE library
# Option 1: If JUCE is installed locally in a 'JUCE' subdirectory
# add_subdirectory(JUCE)

# Option 2: Download JUCE automatically from GitHub (recommended for beginners)
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.9  # Use a stable version
)
FetchContent_MakeAvailable(JUCE)

# Define the target application
juce_add_gui_app(AudioPlayer
    COMPANY_NAME "YourName"
    PRODUCT_NAME "Audio Player"
    VERSION "1.0.0"
    BUNDLE_ID "com.yourname.audioplayer"
)

# Add source files to the target
target_sources(AudioPlayer
    PRIVATE
        Source/Main.cpp
        Source/MainComponent.cpp
        Source/MainComponent.h
)

# Specify which JUCE modules this app needs
target_compile_definitions(AudioPlayer
    PRIVATE
        # JUCE settings
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:AudioPlayer,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:AudioPlayer,JUCE_VERSION>"
)

# Link JUCE modules to the app
target_link_libraries(AudioPlayer
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Platform-specific settings
if(APPLE)
    # macOS specific settings
    set_target_properties(AudioPlayer PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourname.audioplayer"
        MACOSX_BUNDLE_BUNDLE_NAME "Audio Player"
    )
endif()

if(WIN32)
    # Windows specific settings
    set_target_properties(AudioPlayer PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
