juce_add_plugin(SurroundProcessor
    COMPANY_NAME "JUCE"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Surr
    FORMATS VST3 Standalone
    PRODUCT_NAME "Surround Processor"
    JUCE_GENERATE_JUCE_HEADER TRUE)

target_sources(SurroundProcessor
    PRIVATE
        PluginProcessor.cpp
        SurroundProcessor.h)

target_compile_definitions(SurroundProcessor
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(SurroundProcessor
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
