
#include "processor.hpp"
#include "pluginview.hpp"

#include "editor.hpp"

namespace syncroboverb {

Editor::Editor (Processor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setLookAndFeel (&lookAndFeel);
    setOpaque (true);
    view = std::make_unique<PluginView>();
    addAndMakeVisible (view.get());
    view->stabilizeComponents (p.getState());
    setSize (view->getWidth(), view->getHeight());
    startTimer (40);
}

Editor::~Editor()
{
    setLookAndFeel (nullptr);
    stopTimer();
}

void Editor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void Editor::resized()
{
    view->setBounds (getLocalBounds());
}

void Editor::timerCallback()
{
    view->setSphereValue (processor.getRMS());
    view->updateParameterValueDisplays ();
    processor.processPendingUIUpdates(); // Process any pending switch updates
}
}
