cmake_minimum_required(VERSION 3.22)
project(ScreechForge VERSION 0.1.0 LANGUAGES C CXX)
include(FetchContent)
FetchContent_Declare(JUCE GIT_REPOSITORY https://github.com/juce-framework/JUCE.git GIT_TAG 8.0.6 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)
juce_add_plugin(ScreechForge COMPANY_NAME "ScreechForge" BUNDLE_ID "audio.screechforge.synth"
    PLUGIN_MANUFACTURER_CODE Scfg PLUGIN_CODE Scrf PRODUCT_NAME "ScreechForge"
    IS_SYNTH TRUE NEEDS_MIDI_INPUT TRUE NEEDS_MIDI_OUTPUT FALSE
    FORMATS VST3 Standalone COPY_PLUGIN_AFTER_BUILD FALSE)
target_sources(ScreechForge PRIVATE Source/Plugin.cpp)
target_compile_features(ScreechForge PRIVATE cxx_std_17)
target_compile_definitions(ScreechForge PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(ScreechForge PRIVATE juce::juce_audio_utils PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)
