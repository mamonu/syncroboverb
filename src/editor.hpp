
#pragma once

#include "juce.hpp"
#include <juce_audio_processors/juce_audio_processors.h>

namespace syncroboverb {

class Processor;
class PluginView;

class Editor  : public AudioProcessorEditor,
                private Timer
{
public:
    Editor (Processor&);
    ~Editor();

    void paint (Graphics&) override;
    void resized() override;

private:
    LookAndFeel_V3 lookAndFeel;
    Processor& processor;
    std::unique_ptr<PluginView> view;

    friend class Timer;
    void timerCallback() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

}
