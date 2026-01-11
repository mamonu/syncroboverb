// Copyright (C) 2015-2025  Kushview, LLC <info@kushview.net>
// SPDX-License-Identifier: GPL3-or-later

#include "syncroboverb.hpp"
#include "spherescope.hpp"
#include "aboutbox.hpp"
#include "Version.h"
#include "pluginview.hpp"

namespace syncroboverb {
//==============================================================================
PluginView::PluginView() {
    //[Constructor_pre] You can add your own custom stuff here..
    combs.setRange (0, 8, false);
    allpasses.setRange (0, 4, false);
    //[/Constructor_pre]

    sphere.reset (new SphereScope());
    addAndMakeVisible (sphere.get());
    sphere->setName ("sphere");

    sphere->setBounds (115, 13, 122, 120);  // Moved 5px left and 5px up from (120, 18)

    comb4.reset (new ToggleSwitch ("comb4"));
    addAndMakeVisible (comb4.get());
    comb4->setButtonText (String());
    comb4->addListener (this);

    comb4->setBounds (318, 50, 40, 40);

    frozen.reset (new ToggleButton ("frozen"));
    addAndMakeVisible (frozen.get());
    frozen->setButtonText (TRANS ("Frozen"));
    frozen->addListener (this);
    frozen->setColour (ToggleButton::textColourId, Colour (0xffe4e4e4));

    frozen->setBounds (176, 176, 104, 24);

    roomSizeLabel.reset (new Label ("roomSizeLabel",
                                    TRANS ("Room Size")));
    addAndMakeVisible (roomSizeLabel.get());
    roomSizeLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    roomSizeLabel->setJustificationType (Justification::centred);
    roomSizeLabel->setEditable (false, false, false);
    roomSizeLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    roomSizeLabel->setColour (TextEditor::textColourId, Colours::black);
    roomSizeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    roomSizeLabel->setBounds (13, 159, 56, 24);

    dampingLabel.reset (new Label ("dampingLabel",
                                   TRANS ("Damping")));
    addAndMakeVisible (dampingLabel.get());
    dampingLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    dampingLabel->setJustificationType (Justification::centred);
    dampingLabel->setEditable (false, false, false);
    dampingLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    dampingLabel->setColour (TextEditor::textColourId, Colours::black);
    dampingLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    dampingLabel->setBounds (72, 159, 56, 24);

    wetLabel.reset (new Label ("wetLabel",
                               TRANS ("Wet")));
    addAndMakeVisible (wetLabel.get());
    wetLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    wetLabel->setJustificationType (Justification::centred);
    wetLabel->setEditable (false, false, false);
    wetLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    wetLabel->setColour (TextEditor::textColourId, Colours::black);
    wetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    wetLabel->setBounds (14, 94, 56, 24);

    dryLabel.reset (new Label ("dryLabel",
                               TRANS ("Dry")));
    addAndMakeVisible (dryLabel.get());
    dryLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    dryLabel->setJustificationType (Justification::centred);
    dryLabel->setEditable (false, false, false);
    dryLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    dryLabel->setColour (TextEditor::textColourId, Colours::black);
    dryLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    dryLabel->setBounds (64, 94, 72, 24);

    widthLabel.reset (new Label ("widthLabel",
                                 TRANS ("Width")));
    addAndMakeVisible (widthLabel.get());
    widthLabel->setTooltip (TRANS ("Width"));
    widthLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    widthLabel->setJustificationType (Justification::centred);
    widthLabel->setEditable (false, false, false);
    widthLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    widthLabel->setColour (TextEditor::textColourId, Colours::black);
    widthLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    widthLabel->setBounds (131, 159, 56, 24);

    comb1.reset (new ToggleSwitch ("comb1"));
    addAndMakeVisible (comb1.get());
    comb1->setButtonText (String());
    comb1->addListener (this);

    comb1->setBounds (219, 50, 40, 40);

    comb2.reset (new ToggleSwitch ("comb2"));
    addAndMakeVisible (comb2.get());
    comb2->setButtonText (String());
    comb2->addListener (this);

    comb2->setBounds (252, 50, 40, 40);

    comb5.reset (new ToggleSwitch ("comb5"));
    addAndMakeVisible (comb5.get());
    comb5->setButtonText (String());
    comb5->addListener (this);

    comb5->setBounds (219, 87, 40, 40);

    comb6.reset (new ToggleSwitch ("comb6"));
    addAndMakeVisible (comb6.get());
    comb6->setButtonText (String());
    comb6->addListener (this);

    comb6->setBounds (252, 87, 40, 40);

    comb3.reset (new ToggleSwitch ("comb3"));
    addAndMakeVisible (comb3.get());
    comb3->setButtonText (String());
    comb3->addListener (this);

    comb3->setBounds (285, 50, 40, 40);

    comb7.reset (new ToggleSwitch ("comb7"));
    addAndMakeVisible (comb7.get());
    comb7->setButtonText (String());
    comb7->addListener (this);

    comb7->setBounds (285, 87, 40, 40);

    comb8.reset (new ToggleSwitch ("comb8"));
    addAndMakeVisible (comb8.get());
    comb8->setButtonText (String());
    comb8->addListener (this);

    comb8->setBounds (318, 87, 40, 40);

    allpass1.reset (new ToggleSwitch ("allpass1"));
    addAndMakeVisible (allpass1.get());
    allpass1->setButtonText (String());
    allpass1->addListener (this);

    allpass1->setBounds (219, 124, 40, 40);

    allpass2.reset (new ToggleSwitch ("allpass2"));
    addAndMakeVisible (allpass2.get());
    allpass2->setButtonText (String());
    allpass2->addListener (this);

    allpass2->setBounds (252, 124, 40, 40);

    allpass3.reset (new ToggleSwitch ("allpass3"));
    addAndMakeVisible (allpass3.get());
    allpass3->setButtonText (String());
    allpass3->addListener (this);

    allpass3->setBounds (285, 124, 40, 40);

    allpass4.reset (new ToggleSwitch ("allpass4"));
    addAndMakeVisible (allpass4.get());
    allpass4->setButtonText (String());
    allpass4->addListener (this);

    allpass4->setBounds (318, 124, 40, 40);

    label.reset (new Label ("new label",
                            TRANS ("Chambers\n")));
    addAndMakeVisible (label.get());
    label->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colour (0xe4dfddaf));
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (221, 159, 136, 24);

    label3.reset (new Label ("new label",
                             TRANS ("SYNC_ROBO_VERB")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (FontOptions (16.00f, Font::plain)).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (Label::textColourId, Colour (0xd9dfddaf));
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (270, 20, 120, 24);

    helpButton.reset (new TextButton ("helpButton"));
    addAndMakeVisible (helpButton.get());
    helpButton->setButtonText (TRANS ("?"));
    helpButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    helpButton->addListener (this);
    helpButton->setColour (TextButton::buttonColourId, Colour (0xe1111111));
    helpButton->setColour (TextButton::buttonOnColourId, Colour (0xff111111));
    helpButton->setColour (TextButton::textColourOffId, Colour (0xe7e5e5e5));
    helpButton->setColour (TextButton::textColourOnId, Colour (0xdee5e5e5));

    helpButton->setBounds (9, 23, 18, 18);

    wetLevel.reset (new SkinDial ("wetLevel"));
    addAndMakeVisible (wetLevel.get());
    wetLevel->setRange (0, 1, 0);
    wetLevel->setSliderStyle (Slider::RotaryVerticalDrag);
    wetLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    wetLevel->addListener (this);

    wetLevel->setBounds (14, 46, 56, 56);

    dryLevel.reset (new SkinDial ("dryLevel"));
    addAndMakeVisible (dryLevel.get());
    dryLevel->setRange (0, 1, 0);
    dryLevel->setSliderStyle (Slider::RotaryVerticalDrag);
    dryLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dryLevel->addListener (this);

    dryLevel->setBounds (73, 46, 56, 56);

    roomSize.reset (new SkinDial ("roomSize"));
    addAndMakeVisible (roomSize.get());
    roomSize->setRange (0, 1, 0);
    roomSize->setSliderStyle (Slider::RotaryVerticalDrag);
    roomSize->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    roomSize->addListener (this);

    roomSize->setBounds (13, 111, 56, 56);

    damping.reset (new SkinDial ("damping"));
    addAndMakeVisible (damping.get());
    damping->setRange (0, 1, 0);
    damping->setSliderStyle (Slider::RotaryVerticalDrag);
    damping->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    damping->addListener (this);

    damping->setBounds (72, 111, 56, 56);

    width.reset (new SkinDial ("width"));
    addAndMakeVisible (width.get());
    width->setRange (0, 1, 0);
    width->setSliderStyle (Slider::RotaryVerticalDrag);
    width->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    width->addListener (this);

    width->setBounds (131, 111, 56, 56);

    // Randomization controls
    randomEnabled.reset (new ToggleSwitch ("randomEnabled"));
    addAndMakeVisible (randomEnabled.get());
    randomEnabled->setButtonText (String());
    randomEnabled->addListener (this);
    randomEnabled->setBounds (380, 50, 40, 40);

    randomEnabledLabel.reset (new Label ("randomEnabledLabel", TRANS ("Random")));
    addAndMakeVisible (randomEnabledLabel.get());
    randomEnabledLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    randomEnabledLabel->setJustificationType (Justification::centred);
    randomEnabledLabel->setEditable (false, false, false);
    randomEnabledLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    randomEnabledLabel->setColour (TextEditor::textColourId, Colours::black);
    randomEnabledLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    randomEnabledLabel->setBounds (360, 94, 80, 24);

    randomRate.reset (new SkinDial ("randomRate"));
    addAndMakeVisible (randomRate.get());
    randomRate->setRange (0, 7, 1);
    randomRate->setSliderStyle (Slider::RotaryVerticalDrag);
    randomRate->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    randomRate->addListener (this);
    randomRate->setBounds (460, 46, 56, 56);

    randomRateLabel.reset (new Label ("randomRateLabel", TRANS ("Rate")));
    addAndMakeVisible (randomRateLabel.get());
    randomRateLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    randomRateLabel->setJustificationType (Justification::centred);
    randomRateLabel->setEditable (false, false, false);
    randomRateLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    randomRateLabel->setColour (TextEditor::textColourId, Colours::black);
    randomRateLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    randomRateLabel->setBounds (460, 104, 56, 24);

    randomAmount.reset (new SkinDial ("randomAmount"));
    addAndMakeVisible (randomAmount.get());
    randomAmount->setRange (0, 1, 0);
    randomAmount->setSliderStyle (Slider::RotaryVerticalDrag);
    randomAmount->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    randomAmount->addListener (this);
    randomAmount->setBounds (540, 46, 56, 56);

    randomAmountLabel.reset (new Label ("randomAmountLabel", TRANS ("Amount")));
    addAndMakeVisible (randomAmountLabel.get());
    randomAmountLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    randomAmountLabel->setJustificationType (Justification::centred);
    randomAmountLabel->setEditable (false, false, false);
    randomAmountLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    randomAmountLabel->setColour (TextEditor::textColourId, Colours::black);
    randomAmountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    randomAmountLabel->setBounds (540, 104, 56, 24);

    randomFilters.reset (new SkinDial ("randomFilters"));
    addAndMakeVisible (randomFilters.get());
    randomFilters->setRange (0, 2, 1);
    randomFilters->setValue (2.0, dontSendNotification);  // Default to BOTH
    randomFilters->setSliderStyle (Slider::RotaryVerticalDrag);
    randomFilters->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    randomFilters->addListener (this);
    randomFilters->setBounds (620, 46, 56, 56);

    randomFiltersLabel.reset (new Label ("randomFiltersLabel", TRANS ("Filters")));
    addAndMakeVisible (randomFiltersLabel.get());
    randomFiltersLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    randomFiltersLabel->setJustificationType (Justification::centred);
    randomFiltersLabel->setEditable (false, false, false);
    randomFiltersLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    randomFiltersLabel->setColour (TextEditor::textColourId, Colours::black);
    randomFiltersLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    randomFiltersLabel->setBounds (620, 104, 56, 24);

    randomRateValueLabel.reset (new Label ("randomRateValueLabel", TRANS ("1/4")));
    addAndMakeVisible (randomRateValueLabel.get());
    randomRateValueLabel->setFont (Font (FontOptions (10.00f, Font::plain)).withTypefaceStyle ("Regular"));
    randomRateValueLabel->setJustificationType (Justification::centred);
    randomRateValueLabel->setEditable (false, false, false);
    randomRateValueLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    randomRateValueLabel->setColour (TextEditor::textColourId, Colours::black);
    randomRateValueLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    randomRateValueLabel->setBounds (460, 120, 56, 16);

    randomAmountValueLabel.reset (new Label ("randomAmountValueLabel", TRANS ("50%")));
    addAndMakeVisible (randomAmountValueLabel.get());
    randomAmountValueLabel->setFont (Font (FontOptions (10.00f, Font::plain)).withTypefaceStyle ("Regular"));
    randomAmountValueLabel->setJustificationType (Justification::centred);
    randomAmountValueLabel->setEditable (false, false, false);
    randomAmountValueLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    randomAmountValueLabel->setColour (TextEditor::textColourId, Colours::black);
    randomAmountValueLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    randomAmountValueLabel->setBounds (540, 120, 56, 16);

    randomFiltersValueLabel.reset (new Label ("randomFiltersValueLabel", TRANS ("BOTH")));
    addAndMakeVisible (randomFiltersValueLabel.get());
    randomFiltersValueLabel->setFont (Font (FontOptions (10.00f, Font::plain)).withTypefaceStyle ("Regular"));
    randomFiltersValueLabel->setJustificationType (Justification::centred);
    randomFiltersValueLabel->setEditable (false, false, false);
    randomFiltersValueLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    randomFiltersValueLabel->setColour (TextEditor::textColourId, Colours::black);
    randomFiltersValueLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    randomFiltersValueLabel->setBounds (620, 120, 56, 16);

    // Crossfade control
    crossfadeRate.reset (new SkinDial ("crossfadeRate"));
    addAndMakeVisible (crossfadeRate.get());
    crossfadeRate->setRange (0, 5, 1);
    crossfadeRate->setValue (2.0, dontSendNotification);  // Default to 1/32 note
    crossfadeRate->setSliderStyle (Slider::RotaryVerticalDrag);
    crossfadeRate->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    crossfadeRate->addListener (this);
    crossfadeRate->setBounds (380, 146, 56, 56);

    crossfadeRateLabel.reset (new Label ("crossfadeRateLabel", TRANS ("Crossfade")));
    addAndMakeVisible (crossfadeRateLabel.get());
    crossfadeRateLabel->setFont (Font (FontOptions (12.00f, Font::plain)).withTypefaceStyle ("Regular"));
    crossfadeRateLabel->setJustificationType (Justification::centred);
    crossfadeRateLabel->setEditable (false, false, false);
    crossfadeRateLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    crossfadeRateLabel->setColour (TextEditor::textColourId, Colours::black);
    crossfadeRateLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    crossfadeRateLabel->setBounds (440, 146, 80, 24);

    crossfadeRateValueLabel.reset (new Label ("crossfadeRateValueLabel", TRANS ("1/32")));
    addAndMakeVisible (crossfadeRateValueLabel.get());
    crossfadeRateValueLabel->setFont (Font (FontOptions (10.00f, Font::plain)).withTypefaceStyle ("Regular"));
    crossfadeRateValueLabel->setJustificationType (Justification::centred);
    crossfadeRateValueLabel->setEditable (false, false, false);
    crossfadeRateValueLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    crossfadeRateValueLabel->setColour (TextEditor::textColourId, Colours::black);
    crossfadeRateValueLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    crossfadeRateValueLabel->setBounds (440, 170, 80, 16);

    randomEnabledStateLabel.reset (new Label ("randomEnabledStateLabel", TRANS ("OFF")));
    addAndMakeVisible (randomEnabledStateLabel.get());
    randomEnabledStateLabel->setFont (Font (FontOptions (10.00f, Font::plain)).withTypefaceStyle ("Regular"));
    randomEnabledStateLabel->setJustificationType (Justification::centred);
    randomEnabledStateLabel->setEditable (false, false, false);
    randomEnabledStateLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    randomEnabledStateLabel->setColour (TextEditor::textColourId, Colours::black);
    randomEnabledStateLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    randomEnabledStateLabel->setBounds (380, 110, 40, 16);

    // Preset buttons
    presetA.reset (new TextButton ("presetA"));
    addAndMakeVisible (presetA.get());
    presetA->setButtonText ("A");
    presetA->addListener (this);
    presetA->setColour (TextButton::buttonColourId, Colour (0xff3a3a3a));
    presetA->setColour (TextButton::textColourOffId, Colour (0xffe5e5e5));
    presetA->setBounds (492, 10, 28, 24);

    presetB.reset (new TextButton ("presetB"));
    addAndMakeVisible (presetB.get());
    presetB->setButtonText ("B");
    presetB->addListener (this);
    presetB->setColour (TextButton::buttonColourId, Colour (0xff3a3a3a));
    presetB->setColour (TextButton::textColourOffId, Colour (0xffe5e5e5));
    presetB->setBounds (524, 10, 28, 24);

    presetC.reset (new TextButton ("presetC"));
    addAndMakeVisible (presetC.get());
    presetC->setButtonText ("C");
    presetC->addListener (this);
    presetC->setColour (TextButton::buttonColourId, Colour (0xff3a3a3a));
    presetC->setColour (TextButton::textColourOffId, Colour (0xffe5e5e5));
    presetC->setBounds (556, 10, 28, 24);

    presetD.reset (new TextButton ("presetD"));
    addAndMakeVisible (presetD.get());
    presetD->setButtonText ("D");
    presetD->addListener (this);
    presetD->setColour (TextButton::buttonColourId, Colour (0xff3a3a3a));
    presetD->setColour (TextButton::textColourOffId, Colour (0xffe5e5e5));
    presetD->setBounds (588, 10, 28, 24);

    presetE.reset (new TextButton ("presetE"));
    addAndMakeVisible (presetE.get());
    presetE->setButtonText ("E");
    presetE->addListener (this);
    presetE->setColour (TextButton::buttonColourId, Colour (0xff3a3a3a));
    presetE->setColour (TextButton::textColourOffId, Colour (0xffe5e5e5));
    presetE->setBounds (620, 10, 28, 24);

    presetF.reset (new TextButton ("presetF"));
    addAndMakeVisible (presetF.get());
    presetF->setButtonText ("F");
    presetF->addListener (this);
    presetF->setColour (TextButton::buttonColourId, Colour (0xff3a3a3a));
    presetF->setColour (TextButton::textColourOffId, Colour (0xffe5e5e5));
    presetF->setBounds (652, 10, 28, 24);

    presetG.reset (new TextButton ("presetG"));
    addAndMakeVisible (presetG.get());
    presetG->setButtonText ("G");
    presetG->addListener (this);
    presetG->setColour (TextButton::buttonColourId, Colour (0xff3a3a3a));
    presetG->setColour (TextButton::textColourOffId, Colour (0xffe5e5e5));
    presetG->setBounds (684, 10, 28, 24);

    drawable1 = Drawable::createFromImageData (res::syncroboverb_bg_png, res::syncroboverb_bg_pngSize);

    //[UserPreSize]
    combButtons.add (comb1.get());
    combButtons.add (comb2.get());
    combButtons.add (comb3.get());
    combButtons.add (comb4.get());
    combButtons.add (comb5.get());
    combButtons.add (comb6.get());
    combButtons.add (comb7.get());
    combButtons.add (comb8.get());
    allPassButtons.add (allpass1.get());
    allPassButtons.add (allpass2.get());
    allPassButtons.add (allpass3.get());
    allPassButtons.add (allpass4.get());

    frozen->setVisible (false);

    for (int i = 0; i < getNumChildComponents(); ++i) {
        if (SkinDial* sd = dynamic_cast<SkinDial*> (getChildComponent (i))) {
            sd->setScale (2);
            sd->setImage (ImageCache::getFromMemory (res::red_knob_png, res::red_knob_pngSize));
        }
    }
    //[/UserPreSize]

    setSize (720, 220);

    //[Constructor] You can add your own custom stuff here..
    about.setPluginName ("SYNC_ROBO_VERB");
    about.setPluginVersion (String("v") + String(PLUGIN_VERSION_STRING));
    about.setPluginUrl ("mamonulabs", "https://mamonulabs.github.io");
    pluginState.addListener (this);
    //[/Constructor]
}

PluginView::~PluginView() {
    //[Destructor_pre]. You can add your own custom destruction code here..
    pluginState.removeListener (this);
    combButtons.clear();
    allPassButtons.clear();
    //[/Destructor_pre]

    sphere = nullptr;
    comb4 = nullptr;
    frozen = nullptr;
    roomSizeLabel = nullptr;
    dampingLabel = nullptr;
    wetLabel = nullptr;
    dryLabel = nullptr;
    widthLabel = nullptr;
    comb1 = nullptr;
    comb2 = nullptr;
    comb5 = nullptr;
    comb6 = nullptr;
    comb3 = nullptr;
    comb7 = nullptr;
    comb8 = nullptr;
    allpass1 = nullptr;
    allpass2 = nullptr;
    allpass3 = nullptr;
    allpass4 = nullptr;
    label = nullptr;
    label3 = nullptr;
    helpButton = nullptr;
    wetLevel = nullptr;
    dryLevel = nullptr;
    roomSize = nullptr;
    damping = nullptr;
    width = nullptr;
    drawable1 = nullptr;

    // Randomization controls
    randomEnabled = nullptr;
    randomRate = nullptr;
    randomAmount = nullptr;
    randomFilters = nullptr;
    randomEnabledLabel = nullptr;
    randomRateLabel = nullptr;
    randomAmountLabel = nullptr;
    randomFiltersLabel = nullptr;
    randomRateValueLabel = nullptr;
    randomAmountValueLabel = nullptr;
    randomFiltersValueLabel = nullptr;
    randomEnabledStateLabel = nullptr;
    
    // Crossfade controls
    crossfadeRate = nullptr;
    crossfadeRateLabel = nullptr;
    crossfadeRateValueLabel = nullptr;

    // Preset buttons
    presetA = nullptr;
    presetB = nullptr;
    presetC = nullptr;
    presetD = nullptr;
    presetE = nullptr;
    presetF = nullptr;
    presetG = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PluginView::paint (Graphics& g) {
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff2d2b24));

    {
        int x = 0, y = 0, w = proportionOfWidth (1.0000f), h = proportionOfHeight (1.0000f);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable1 != nullptr);
        if (drawable1 != nullptr)
            drawable1->drawWithin (g, Rectangle<float> (x, y, w, h), RectanglePlacement::stretchToFit, 1.000f);
    }


    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PluginView::resized() {
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PluginView::buttonClicked (Button* buttonThatWasClicked) {
    //[UserbuttonClicked_Pre]
    if (combButtons.contains (buttonThatWasClicked)) {
        combs.setBit (combButtons.indexOf (buttonThatWasClicked),
                      buttonThatWasClicked->getToggleState());
        pluginState.setProperty ("enabledCombs", combs.toString (2), nullptr);
        return;
    } else if (allPassButtons.contains (buttonThatWasClicked)) {
        allpasses.setBit (allPassButtons.indexOf (buttonThatWasClicked),
                          buttonThatWasClicked->getToggleState());
        pluginState.setProperty ("enabledAllPasses", allpasses.toString (2), nullptr);
        return;
    }
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == comb4.get()) {
        //[UserButtonCode_comb4] -- add your button handler code here..
        //[/UserButtonCode_comb4]
    } else if (buttonThatWasClicked == frozen.get()) {
        //[UserButtonCode_frozen] -- add your button handler code here..
        //[/UserButtonCode_frozen]
    } else if (buttonThatWasClicked == comb1.get()) {
        //[UserButtonCode_comb1] -- add your button handler code here..
        //[/UserButtonCode_comb1]
    } else if (buttonThatWasClicked == comb2.get()) {
        //[UserButtonCode_comb2] -- add your button handler code here..
        //[/UserButtonCode_comb2]
    } else if (buttonThatWasClicked == comb5.get()) {
        //[UserButtonCode_comb5] -- add your button handler code here..
        //[/UserButtonCode_comb5]
    } else if (buttonThatWasClicked == comb6.get()) {
        //[UserButtonCode_comb6] -- add your button handler code here..
        //[/UserButtonCode_comb6]
    } else if (buttonThatWasClicked == comb3.get()) {
        //[UserButtonCode_comb3] -- add your button handler code here..
        //[/UserButtonCode_comb3]
    } else if (buttonThatWasClicked == comb7.get()) {
        //[UserButtonCode_comb7] -- add your button handler code here..
        //[/UserButtonCode_comb7]
    } else if (buttonThatWasClicked == comb8.get()) {
        //[UserButtonCode_comb8] -- add your button handler code here..
        //[/UserButtonCode_comb8]
    } else if (buttonThatWasClicked == allpass1.get()) {
        //[UserButtonCode_allpass1] -- add your button handler code here..
        //[/UserButtonCode_allpass1]
    } else if (buttonThatWasClicked == allpass2.get()) {
        //[UserButtonCode_allpass2] -- add your button handler code here..
        //[/UserButtonCode_allpass2]
    } else if (buttonThatWasClicked == allpass3.get()) {
        //[UserButtonCode_allpass3] -- add your button handler code here..
        //[/UserButtonCode_allpass3]
    } else if (buttonThatWasClicked == allpass4.get()) {
        //[UserButtonCode_allpass4] -- add your button handler code here..
        //[/UserButtonCode_allpass4]
    } else if (buttonThatWasClicked == helpButton.get()) {
        //[UserButtonCode_helpButton] -- add your button handler code here..
        if (! about.isVisible()) {
            addAndMakeVisible (about);
            about.setVisible (true);
            about.centreWithSize (about.getWidth(), about.getHeight());
        } else {
            removeChildComponent (&about);
            about.setVisible (false);
        }
        //[/UserButtonCode_helpButton]
    } else if (buttonThatWasClicked == randomEnabled.get()) {
        //[UserButtonCode_randomEnabled] -- add your button handler code here..
        pluginState.setProperty (Tags::randomEnabled, randomEnabled->getToggleState() ? 1.0f : 0.0f, nullptr);
        updateParameterValueDisplays ();
        //[/UserButtonCode_randomEnabled]
    } else if (buttonThatWasClicked == presetA.get()) {
        applyPresetA();
    } else if (buttonThatWasClicked == presetB.get()) {
        applyPresetB();
    } else if (buttonThatWasClicked == presetC.get()) {
        applyPresetC();
    } else if (buttonThatWasClicked == presetD.get()) {
        applyPresetD();
    } else if (buttonThatWasClicked == presetE.get()) {
        applyPresetE();
    } else if (buttonThatWasClicked == presetF.get()) {
        applyPresetF();
    } else if (buttonThatWasClicked == presetG.get()) {
        applyPresetG();
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PluginView::sliderValueChanged (Slider* sliderThatWasMoved) {
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == wetLevel.get()) {
        //[UserSliderCode_wetLevel] -- add your slider handling code here..
        //[/UserSliderCode_wetLevel]
    } else if (sliderThatWasMoved == dryLevel.get()) {
        //[UserSliderCode_dryLevel] -- add your slider handling code here..
        //[/UserSliderCode_dryLevel]
    } else if (sliderThatWasMoved == roomSize.get()) {
        //[UserSliderCode_roomSize] -- add your slider handling code here..
        //[/UserSliderCode_roomSize]
    } else if (sliderThatWasMoved == damping.get()) {
        //[UserSliderCode_damping] -- add your slider handling code here..
        //[/UserSliderCode_damping]
    } else if (sliderThatWasMoved == width.get()) {
        //[UserSliderCode_width] -- add your slider handling code here..
        //[/UserSliderCode_width]
    } else if (sliderThatWasMoved == randomRate.get()) {
        //[UserSliderCode_randomRate] -- add your slider handling code here..
        pluginState.setProperty (Tags::randomRate, (float)randomRate->getValue(), nullptr);
        updateParameterValueDisplays ();
        //[/UserSliderCode_randomRate]
    } else if (sliderThatWasMoved == randomAmount.get()) {
        //[UserSliderCode_randomAmount] -- add your slider handling code here..
        pluginState.setProperty (Tags::randomAmount, (float)randomAmount->getValue(), nullptr);
        updateParameterValueDisplays ();
        //[/UserSliderCode_randomAmount]
    } else if (sliderThatWasMoved == randomFilters.get()) {
        //[UserSliderCode_randomFilters] -- add your slider handling code here..
        pluginState.setProperty (Tags::randomFilters, (float)randomFilters->getValue(), nullptr);
        updateParameterValueDisplays ();
        //[/UserSliderCode_randomFilters]
    } else if (sliderThatWasMoved == crossfadeRate.get()) {
        //[UserSliderCode_crossfadeRate] -- add your slider handling code here..
        pluginState.setProperty (Tags::crossfadeRate, (float)crossfadeRate->getValue(), nullptr);
        updateParameterValueDisplays ();
        //[/UserSliderCode_crossfadeRate]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void PluginView::stabilizeComponents (ValueTree newPluginState) {
    pluginState = newPluginState;

    roomSize->setValue (pluginState.getProperty ("roomSize"), dontSendNotification);
    roomSize->getValueObject().referTo (
        pluginState.getPropertyAsValue ("roomSize", nullptr));

    dryLevel->setValue (pluginState.getProperty ("dryLevel"), dontSendNotification);
    dryLevel->getValueObject().referTo (
        pluginState.getPropertyAsValue ("dryLevel", nullptr));

    wetLevel->setValue (pluginState.getProperty ("wetLevel"), dontSendNotification);
    wetLevel->getValueObject().referTo (
        pluginState.getPropertyAsValue ("wetLevel", nullptr));

    width->setValue (pluginState.getProperty ("width"), dontSendNotification);
    width->getValueObject().referTo (
        pluginState.getPropertyAsValue ("width", nullptr));

    damping->setValue (pluginState.getProperty ("damping"), dontSendNotification);
    damping->getValueObject().referTo (
        pluginState.getPropertyAsValue ("damping", nullptr));

    combs.parseString (pluginState.getProperty (Tags::enabledCombs).toString(), 2);
    allpasses.parseString (pluginState.getProperty (Tags::enabledAllPasses).toString(), 2);

    for (int i = 0; i < combButtons.size(); ++i)
        combButtons.getUnchecked (i)->setToggleState (combs[i], dontSendNotification);
    for (int i = 0; i < allPassButtons.size(); ++i)
        allPassButtons.getUnchecked (i)->setToggleState (allpasses[i], dontSendNotification);

    // Randomization controls
    randomEnabled->setToggleState ((float)pluginState.getProperty (Tags::randomEnabled) > 0.5f, dontSendNotification);
    randomEnabled->getToggleStateValue().referTo (
        pluginState.getPropertyAsValue (Tags::randomEnabled, nullptr));

    randomRate->setValue (pluginState.getProperty (Tags::randomRate), dontSendNotification);
    randomRate->getValueObject().referTo (
        pluginState.getPropertyAsValue (Tags::randomRate, nullptr));

    randomAmount->setValue (pluginState.getProperty (Tags::randomAmount), dontSendNotification);
    randomAmount->getValueObject().referTo (
        pluginState.getPropertyAsValue (Tags::randomAmount, nullptr));

    randomFilters->setValue (pluginState.getProperty (Tags::randomFilters), dontSendNotification);
    randomFilters->getValueObject().referTo (
        pluginState.getPropertyAsValue (Tags::randomFilters, nullptr));

    crossfadeRate->setValue (pluginState.getProperty (Tags::crossfadeRate), dontSendNotification);
    crossfadeRate->getValueObject().referTo (
        pluginState.getPropertyAsValue (Tags::crossfadeRate, nullptr));

    // Update parameter value displays
    updateParameterValueDisplays ();
}

void PluginView::valueTreePropertyChanged (ValueTree& tree, const Identifier& property) {
    if (tree != pluginState)
        return;

    const var& value (tree.getProperty (property));
    if (property == Tags::enabledCombs) {
        combs.parseString (value.toString(), 2);
        for (int i = 0; i < combButtons.size(); ++i)
            combButtons.getUnchecked (i)->setToggleState (combs[i], dontSendNotification);
    } else if (property == Tags::enabledAllPasses) {
        allpasses.parseString (value.toString(), 2);
        for (int i = 0; i < allPassButtons.size(); ++i)
            allPassButtons.getUnchecked (i)->setToggleState (allpasses[i], dontSendNotification);
    } else if (property == Tags::randomEnabled) {
        randomEnabled->setToggleState ((float)value > 0.5f, dontSendNotification);
    } else if (property == Tags::randomRate) {
        randomRate->setValue (value, dontSendNotification);
    } else if (property == Tags::randomAmount) {
        randomAmount->setValue (value, dontSendNotification);
    } else if (property == Tags::randomFilters) {
        randomFilters->setValue (value, dontSendNotification);
    } else if (property == Tags::crossfadeRate) {
        crossfadeRate->setValue (value, dontSendNotification);
    }
}

void PluginView::setSphereValue (const float val) {
    sphere->setValue (val);
    sphere->repaint();
}

void PluginView::updateParameterValueDisplays () {
    // Update rate display
    int rateValue = (int)randomRate->getValue();
    String rateText;
    switch (rateValue) {
        case 0: rateText = "1/16"; break;
        case 1: rateText = "1/8"; break;
        case 2: rateText = "1/4"; break;
        case 3: rateText = "1/2"; break;
        case 4: rateText = "1"; break;
        case 5: rateText = "2 bars"; break;
        case 6: rateText = "4 bars"; break;
        case 7: rateText = "8 bars"; break;
        default: rateText = "1/4"; break;
    }
    randomRateValueLabel->setText (rateText, dontSendNotification);

    // Update amount display  
    int amountPercent = (int)(randomAmount->getValue() * 100.0f);
    String amountText = String(amountPercent) + "%";
    randomAmountValueLabel->setText (amountText, dontSendNotification);

    // Update filters display
    int filtersValue = (int)randomFilters->getValue();
    String filtersText;
    switch (filtersValue) {
        case 0: filtersText = "COMB"; break;
        case 1: filtersText = "ALLPASS"; break;
        case 2: filtersText = "BOTH"; break;
        default: filtersText = "BOTH"; break;
    }
    randomFiltersValueLabel->setText (filtersText, dontSendNotification);

    // Update crossfade display
    int crossfadeValue = (int)crossfadeRate->getValue();
    String crossfadeText;
    switch (crossfadeValue) {
        case 0: crossfadeText = "INSTANT"; break;
        case 1: crossfadeText = "1/64"; break;
        case 2: crossfadeText = "1/32"; break;
        case 3: crossfadeText = "1/16"; break;
        case 4: crossfadeText = "1/8"; break;
        case 5: crossfadeText = "1/4"; break;
        default: crossfadeText = "1/32"; break;
    }
    crossfadeRateValueLabel->setText (crossfadeText, dontSendNotification);

    // Update enabled state display
    String stateText = randomEnabled->getToggleState() ? "ON" : "OFF";
    randomEnabledStateLabel->setText (stateText, dontSendNotification);
}

void PluginView::mouseDown (const MouseEvent& ev) {
    if (about.isVisible())
        about.setVisible (false);
    Component::mouseDown (ev);
}

void PluginView::applyPresetA() {
    // A: RATE 1/4, small size, small width, filters COMB
    pluginState.setProperty (Tags::randomRate, 2.0f, nullptr);      // 1/4 note
    pluginState.setProperty (Tags::roomSize, 0.25f, nullptr);       // small size
    pluginState.setProperty (Tags::width, 0.25f, nullptr);          // small width
    pluginState.setProperty (Tags::randomFilters, 0.0f, nullptr);   // COMB only
    updateParameterValueDisplays();
}

void PluginView::applyPresetB() {
    // B: RATE 1 bar, large size, filters COMB
    pluginState.setProperty (Tags::randomRate, 4.0f, nullptr);      // 1 bar (whole note)
    pluginState.setProperty (Tags::roomSize, 0.8f, nullptr);        // large size
    pluginState.setProperty (Tags::randomFilters, 0.0f, nullptr);   // COMB only
    updateParameterValueDisplays();
}

void PluginView::applyPresetC() {
    // C: RATE 2 bars, large size, filters ALL (Both)
    pluginState.setProperty (Tags::randomRate, 5.0f, nullptr);      // 2 bars
    pluginState.setProperty (Tags::roomSize, 0.8f, nullptr);        // large size
    pluginState.setProperty (Tags::randomFilters, 2.0f, nullptr);   // BOTH
    updateParameterValueDisplays();
}

void PluginView::applyPresetD() {
    // D: Shimmer - fast rate, high damping, wide, allpass focus
    pluginState.setProperty (Tags::randomRate, 0.0f, nullptr);      // 1/16 note (fast shimmer)
    pluginState.setProperty (Tags::roomSize, 0.6f, nullptr);        // medium-large size
    pluginState.setProperty (Tags::damping, 0.7f, nullptr);         // high damping for smoothness
    pluginState.setProperty (Tags::width, 0.9f, nullptr);           // wide stereo
    pluginState.setProperty (Tags::randomFilters, 1.0f, nullptr);   // ALLPASS only
    pluginState.setProperty (Tags::randomAmount, 0.3f, nullptr);    // subtle changes
    updateParameterValueDisplays();
}

void PluginView::applyPresetE() {
    // E: Chaos - very fast, high amount, both filters
    pluginState.setProperty (Tags::randomRate, 0.0f, nullptr);      // 1/16 note
    pluginState.setProperty (Tags::roomSize, 0.5f, nullptr);        // medium size
    pluginState.setProperty (Tags::damping, 0.3f, nullptr);         // low damping (bright)
    pluginState.setProperty (Tags::width, 1.0f, nullptr);           // full width
    pluginState.setProperty (Tags::randomFilters, 2.0f, nullptr);   // BOTH
    pluginState.setProperty (Tags::randomAmount, 0.8f, nullptr);    // high randomization
    updateParameterValueDisplays();
}

void PluginView::applyPresetF() {
    // F: Ambient Pad - slow evolution, large space
    pluginState.setProperty (Tags::randomRate, 7.0f, nullptr);      // 8 bars (very slow)
    pluginState.setProperty (Tags::roomSize, 0.95f, nullptr);       // huge room
    pluginState.setProperty (Tags::damping, 0.6f, nullptr);         // warm
    pluginState.setProperty (Tags::width, 0.7f, nullptr);           // moderate width
    pluginState.setProperty (Tags::wetLevel, 0.5f, nullptr);        // more wet
    pluginState.setProperty (Tags::randomFilters, 2.0f, nullptr);   // BOTH
    pluginState.setProperty (Tags::randomAmount, 0.4f, nullptr);    // gentle changes
    updateParameterValueDisplays();
}

void PluginView::applyPresetG() {
    // G: Rhythmic Gate - synced to beat, punchy
    pluginState.setProperty (Tags::randomRate, 1.0f, nullptr);      // 1/8 note
    pluginState.setProperty (Tags::roomSize, 0.4f, nullptr);        // smaller room
    pluginState.setProperty (Tags::damping, 0.2f, nullptr);         // bright/sharp
    pluginState.setProperty (Tags::width, 0.5f, nullptr);           // focused
    pluginState.setProperty (Tags::dryLevel, 0.5f, nullptr);        // more dry
    pluginState.setProperty (Tags::randomFilters, 0.0f, nullptr);   // COMB only
    pluginState.setProperty (Tags::randomAmount, 0.6f, nullptr);    // moderate randomization
    updateParameterValueDisplays();
}

}
