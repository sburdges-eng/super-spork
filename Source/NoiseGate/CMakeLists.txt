juce_add_plugin(NoiseGate
    COMPANY_NAME "JUCE"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Ngt1
    FORMATS VST3 Standalone
    PRODUCT_NAME "Noise Gate"
    JUCE_GENERATE_JUCE_HEADER TRUE)

target_sources(NoiseGate
    PRIVATE
        PluginProcessor.cpp
        NoiseGate.h)

target_compile_definitions(NoiseGate
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(NoiseGate
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
