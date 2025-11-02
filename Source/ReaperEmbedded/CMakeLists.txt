juce_add_plugin(ReaperEmbedded
    COMPANY_NAME "JUCE"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Repr
    FORMATS VST3 Standalone
    PRODUCT_NAME "Reaper Embedded View Demo"
    JUCE_GENERATE_JUCE_HEADER TRUE)

target_sources(ReaperEmbedded
    PRIVATE
        PluginProcessor.cpp
        ReaperEmbedded.h
        extern/reaper_plugin_fx_embed.h
        extern/reaper_vst3_interfaces.h)

target_compile_definitions(ReaperEmbedded
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(ReaperEmbedded
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
