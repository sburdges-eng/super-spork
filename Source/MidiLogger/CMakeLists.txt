juce_add_plugin(MidiLogger
    COMPANY_NAME "JUCE"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Mdlg
    FORMATS VST3 Standalone
    PRODUCT_NAME "MIDI Logger"
    JUCE_GENERATE_JUCE_HEADER TRUE)

target_sources(MidiLogger
    PRIVATE
        PluginProcessor.cpp
        MidiLogger.h)

target_compile_definitions(MidiLogger
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(MidiLogger
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
