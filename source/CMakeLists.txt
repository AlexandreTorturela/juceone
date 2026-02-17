cmake_minimum_required(VERSION 3.21)

project(JuceOne VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE submodule (path relative to -S source)
add_subdirectory(../external/JUCE JUCE-build)

# Main plugin target
juce_add_plugin(JuceOne
        COMPANY_NAME "JuceOne"
        PRODUCT_NAME "JuceOne"

        # 4 chars each (A-Z, a-z, 0-9). Keep stable once you ship.
        PLUGIN_MANUFACTURER_CODE Juce
        PLUGIN_CODE JcO1

        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE

        COPY_PLUGIN_AFTER_BUILD FALSE

        FORMATS VST3 Standalone
)

# Your files live in source/ (same directory as this CMakeLists)
target_sources(JuceOne
        PRIVATE
        PluginProcessor.cpp
        PluginProcessor.h
        PluginEditor.cpp
        PluginEditor.h
)

# Keep deps minimal
target_compile_definitions(JuceOne
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0

        # Avoid VST2-related build paths/conflicts
        JucePlugin_Build_VST=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(JuceOne
        PRIVATE
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)



# =====================================================================
# Development helpers: copy artefacts to repo-local folders
# =====================================================================

# Repo-local dev folders (one level above /source)
set(VST3_DEV_DIR       "${CMAKE_SOURCE_DIR}/vst3dev")
set(STANDALONE_DEV_DIR "${CMAKE_SOURCE_DIR}/standalonedev")

file(MAKE_DIRECTORY "${VST3_DEV_DIR}")
file(MAKE_DIRECTORY "${STANDALONE_DEV_DIR}")

# Derive names from the main target to avoid hardcoding
set(PLUGIN_TARGET "JuceOne")
set(ARTEFACTS_DIR "${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_TARGET}_artefacts")

# ---- VST3 ------------------------------------------------------------
# JUCE outputs:
#   <build>/${PLUGIN_TARGET}_artefacts/<CONFIG>/VST3/${PLUGIN_TARGET}.vst3
add_custom_command(
        TARGET ${PLUGIN_TARGET}_VST3
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ARTEFACTS_DIR}/$<CONFIG>/VST3/${PLUGIN_TARGET}.vst3"
        "${VST3_DEV_DIR}/${PLUGIN_TARGET}.vst3"
        COMMENT "Copying VST3 to ${VST3_DEV_DIR}"
)

# ---- Standalone ------------------------------------------------------
# JUCE outputs:
#   <build>/${PLUGIN_TARGET}_artefacts/<CONFIG>/Standalone/${PLUGIN_TARGET}.exe
add_custom_command(
        TARGET ${PLUGIN_TARGET}_Standalone
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${ARTEFACTS_DIR}/$<CONFIG>/Standalone/${PLUGIN_TARGET}.exe"
        "${STANDALONE_DEV_DIR}/${PLUGIN_TARGET}.exe"
        COMMENT "Copying Standalone to ${STANDALONE_DEV_DIR}"
)
