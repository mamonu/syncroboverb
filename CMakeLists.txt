# SyncRoboVerb CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(SyncRoboVerb VERSION 0.8.0)

# Build numbering system
if(EXISTS "${CMAKE_SOURCE_DIR}/build_number.txt")
    file(READ "${CMAKE_SOURCE_DIR}/build_number.txt" BUILD_NUMBER)
    string(STRIP "${BUILD_NUMBER}" BUILD_NUMBER)
else()
    set(BUILD_NUMBER "14")
endif()

math(EXPR BUILD_NUMBER "${BUILD_NUMBER} + 1")
file(WRITE "${CMAKE_SOURCE_DIR}/build_number.txt" "${BUILD_NUMBER}")

# Generate timestamp
string(TIMESTAMP BUILD_TIMESTAMP "%Y%m%d_%H%M")

# Configure build info
configure_file("${CMAKE_SOURCE_DIR}/src/buildinfo.h.in" 
               "${CMAKE_BINARY_DIR}/src/buildinfo.h" @ONLY)

add_subdirectory(deps/JUCE)
add_subdirectory(deps/clap-juce-extensions EXCLUDE_FROM_ALL)

juce_add_plugin(SyncRoboVerb
    COMPANY_NAME mamonulabs                     # Specify the name of the plugin's author
    IS_SYNTH FALSE                              # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE                      # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE           # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD FALSE               # Should the plugin be installed to a default location after building?
    LV2URI "mamonulabs.github.io"
    PLUGIN_MANUFACTURER_CODE MnlS               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Ulbs                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "SyncRoboVerb")                        # The name of the final executable, which can differ from the target name
clap_juce_extensions_plugin(TARGET SyncRoboVerb
    CLAP_ID "net.kushview.SyncRoboVerb"
    CLAP_FEATURES reverb)

target_include_directories(SyncRoboVerb PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>)

target_sources(SyncRoboVerb
    PRIVATE
        src/syncroboverb.cpp
        src/processor.cpp
        src/res.cpp
        src/editor.cpp
        src/aboutbox.cpp
        src/pluginview.cpp)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(SyncRoboVerb PUBLIC -Wno-unused-parameter -Wno-overloaded-virtual)
endif()

target_compile_definitions(SyncRoboVerb
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(SyncRoboVerb
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)