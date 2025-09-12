// Copyright (C) 2015-2025  Kushview, LLC <info@kushview.net>
// SPDX-License-Identifier: GPL3-or-later

#include "editor.hpp"
#include "processor.hpp"

namespace syncroboverb {

Processor::Processor()
    : numParameters (SyncRoboVerb::numParameters + 12),
      numKnobs (SyncRoboVerb::numParameters),
      state ("syncroboverb") {
    updateState();
    state.addListener (this);
}

Processor::~Processor() {
    state.removeListener (this);
}

const String Processor::getName() const { return "SyncRoboVerb"; }
int Processor::getNumParameters() {
    // SyncRoboVerb::numParameters includes all reverb parameters + randomization parameters
    // +12 is for the number of switches (8 combs + 4 allpasses)
    return numParameters;
}

float Processor::getParameter (int index) {
    if (index >= numKnobs && index < numParameters) {
        const int filterIndex = index - numKnobs;

        if (filterIndex < 8) {
            return verb.toggledCombFloat (filterIndex);
        } else {
            return verb.toggledAllPassFloat (filterIndex - 8);
        }
    } else {
        const SyncRoboVerb::Parameters& p (verb.getParameters());
        switch (index) {
            case SyncRoboVerb::RoomSize:
                return p.roomSize;
                break;
            case SyncRoboVerb::Damping:
                return p.damping;
                break;
            case SyncRoboVerb::WetLevel:
                return p.wetLevel;
                break;
            case SyncRoboVerb::DryLevel:
                return p.dryLevel;
                break;
            case SyncRoboVerb::Width:
                return p.width;
                break;
            case SyncRoboVerb::FreezeMode:
                return p.freezeMode;
                break;
            case SyncRoboVerb::RandomEnabled:
                return p.randomEnabled;
                break;
            case SyncRoboVerb::RandomRate:
                return p.randomRate;
                break;
            case SyncRoboVerb::RandomAmount:
                return p.randomAmount;
                break;
            case SyncRoboVerb::RandomFilters:
                return p.randomFilters;
                break;
            case SyncRoboVerb::CrossfadeRate:
                return p.crossfadeRate;
                break;
            case SyncRoboVerb::AllPassGain1:
                return p.allPassGain1;
                break;
            case SyncRoboVerb::AllPassGain2:
                return p.allPassGain2;
                break;
            case SyncRoboVerb::AllPassGain3:
                return p.allPassGain3;
                break;
            case SyncRoboVerb::AllPassGain4:
                return p.allPassGain4;
                break;
        }
    }

    return 0.0f;
}

void Processor::setParameter (int index, float newValue) {
    if (index >= numKnobs && index < numParameters) {
        ScopedLock lock (getCallbackLock());
        const bool isSet = (newValue < 0.5) ? false : true;
        const int filterIndex = index - numKnobs;
        
        // Use crossfading for smooth transitions
        bool combStates[8];
        bool allPassStates[4];
        
        // Get current states
        for (int i = 0; i < 8; ++i) {
            combStates[i] = (verb.toggledCombFloat(i) >= 0.5f);
        }
        for (int i = 0; i < 4; ++i) {
            allPassStates[i] = (verb.toggledAllPassFloat(i) >= 0.5f);
        }
        
        // Update the specific switch
        if (filterIndex < 8) {
            combStates[filterIndex] = isSet;
        } else {
            allPassStates[filterIndex - 8] = isSet;
        }
        
        // Apply changes with crossfading
        verb.updateAllFilters(combStates, allPassStates);
    } else {
        switch (index) {
            case SyncRoboVerb::RoomSize:
                params.roomSize = newValue;
                break;
            case SyncRoboVerb::Damping:
                params.damping = newValue;
                break;
            case SyncRoboVerb::WetLevel:
                params.wetLevel = newValue;
                break;
            case SyncRoboVerb::DryLevel:
                params.dryLevel = newValue;
                break;
            case SyncRoboVerb::Width:
                params.width = newValue;
                break;
            case SyncRoboVerb::FreezeMode:
                params.freezeMode = newValue;
                break;
            case SyncRoboVerb::RandomEnabled:
                params.randomEnabled = newValue;
                verb.getRandomizer().setEnabled(newValue >= 0.5f);
                break;
            case SyncRoboVerb::RandomRate:
                params.randomRate = newValue;
                verb.getRandomizer().setRate(static_cast<TempoSyncedRandomizer::RandomRate>(
                    juce::jlimit(0, TempoSyncedRandomizer::numRates - 1, (int)newValue)));
                break;
            case SyncRoboVerb::RandomAmount:
                params.randomAmount = newValue;
                verb.getRandomizer().setAmount(newValue);
                break;
            case SyncRoboVerb::RandomFilters:
                params.randomFilters = newValue;
                verb.getRandomizer().setFilterType(static_cast<TempoSyncedRandomizer::FilterType>(
                    juce::jlimit(0, TempoSyncedRandomizer::numFilterTypes - 1, (int)newValue)));
                break;
            case SyncRoboVerb::CrossfadeRate:
                params.crossfadeRate = newValue;
                verb.getCrossfadeManager().setCrossfadeTiming(static_cast<TempoSyncedCrossfadeManager::CrossfadeTiming>(
                    juce::jlimit(0, TempoSyncedCrossfadeManager::numTimings - 1, (int)newValue)));
                break;
            case SyncRoboVerb::AllPassGain1:
                params.allPassGain1 = juce::jlimit(0.0f, 1.0f, newValue);
                break;
            case SyncRoboVerb::AllPassGain2:
                params.allPassGain2 = juce::jlimit(0.0f, 1.0f, newValue);
                break;
            case SyncRoboVerb::AllPassGain3:
                params.allPassGain3 = juce::jlimit(0.0f, 1.0f, newValue);
                break;
            case SyncRoboVerb::AllPassGain4:
                params.allPassGain4 = juce::jlimit(0.0f, 1.0f, newValue);
                break;
        }

        ScopedLock lock (getCallbackLock());
        if (params != verb.getParameters())
            verb.setParameters (params);
    }
}

String Processor::getParameterName (int index, int) {
    if (index >= numKnobs && index < numParameters) {
        const int filterIndex = index - numKnobs;
        if (filterIndex < 8) {
            return String ("Comb Filter ") + String (filterIndex + 1);
        } else {
            return String ("AllPass Filter ") + String (filterIndex - 8 + 1);
        }
    }

    switch (index) {
        case SyncRoboVerb::RoomSize:
            return "Room size";
            break;
        case SyncRoboVerb::Damping:
            return "Damping";
            break;
        case SyncRoboVerb::WetLevel:
            return "Wet level";
            break;
        case SyncRoboVerb::DryLevel:
            return "Dry level";
            break;
        case SyncRoboVerb::Width:
            return "Width";
            break;
        case SyncRoboVerb::FreezeMode:
            return "Freeze mode";
            break;
        case SyncRoboVerb::RandomEnabled:
            return "Random enabled";
            break;
        case SyncRoboVerb::RandomRate:
            return "Random rate";
            break;
        case SyncRoboVerb::RandomAmount:
            return "Random amount";
            break;
        case SyncRoboVerb::RandomFilters:
            return "Random filters";
            break;
        case SyncRoboVerb::CrossfadeRate:
            return "Crossfade rate";
            break;
        case SyncRoboVerb::AllPassGain1:
            return "AllPass Gain 1";
            break;
        case SyncRoboVerb::AllPassGain2:
            return "AllPass Gain 2";
            break;
        case SyncRoboVerb::AllPassGain3:
            return "AllPass Gain 3";
            break;
        case SyncRoboVerb::AllPassGain4:
            return "AllPass Gain 4";
            break;
    }

    return String();
}

String Processor::getParameterText (int index, int maxLen) {
    if (index >= numKnobs && index < numParameters) {
        getParameterName (index - numKnobs, maxLen);
    }

    switch (index) {
        case SyncRoboVerb::RoomSize:
            return String(params.roomSize, 2);
            break;
        case SyncRoboVerb::Damping:
            return String(params.damping, 2);
            break;
        case SyncRoboVerb::WetLevel:
            return String(params.wetLevel, 2);
            break;
        case SyncRoboVerb::DryLevel:
            return String(params.dryLevel, 2);
            break;
        case SyncRoboVerb::Width:
            return String(params.width, 2);
            break;
        case SyncRoboVerb::FreezeMode:
            return params.freezeMode >= 0.5f ? "On" : "Off";
            break;
        case SyncRoboVerb::RandomEnabled:
            return params.randomEnabled >= 0.5f ? "On" : "Off";
            break;
        case SyncRoboVerb::RandomRate:
        {
            int rateValue = (int)params.randomRate;
            switch (rateValue) {
                case 0: return "1/16";
                case 1: return "1/8";
                case 2: return "1/4";
                case 3: return "1/2";
                case 4: return "1";
                case 5: return "2 bars";
                case 6: return "4 bars";
                case 7: return "8 bars";
                default: return "1/4";
            }
            break;
        }
        case SyncRoboVerb::RandomAmount:
            return String((int)(params.randomAmount * 100.0f)) + "%";
            break;
        case SyncRoboVerb::RandomFilters:
        {
            int filtersValue = (int)params.randomFilters;
            switch (filtersValue) {
                case 0: return "Comb";
                case 1: return "AllPass";
                case 2: return "Both";
                default: return "Both";
            }
            break;
        }
        case SyncRoboVerb::CrossfadeRate:
        {
            int crossfadeValue = (int)params.crossfadeRate;
            switch (crossfadeValue) {
                case 0: return "Instant";
                case 1: return "1/64";
                case 2: return "1/32";
                case 3: return "1/16";
                case 4: return "1/8";
                case 5: return "1/4";
                default: return "1/32";
            }
            break;
        }
    }

    return String();
}

const String Processor::getInputChannelName (int channelIndex) const {
    return String (channelIndex + 1);
}

const String Processor::getOutputChannelName (int channelIndex) const {
    return String (channelIndex + 1);
}

bool Processor::isInputChannelStereoPair (int) const { return true; }
bool Processor::isOutputChannelStereoPair (int) const { return true; }

bool Processor::acceptsMidi() const { return false; }
bool Processor::producesMidi() const { return false; }
bool Processor::silenceInProducesSilenceOut() const { return false; }
double Processor::getTailLengthSeconds() const { return 0.0; }

int Processor::getNumPrograms() { return 1; }
int Processor::getCurrentProgram() { return 0; }
void Processor::setCurrentProgram (int) {}
const String Processor::getProgramName (int) { return String(); }

void Processor::changeProgramName (int index, const String& newName) {
    juce::ignoreUnused (index, newName);
}

void Processor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/) {
    verb.reset();
    verb.setSampleRate (sampleRate);
}

void Processor::releaseResources() {}

void Processor::processBlock (AudioBuffer<double>&, MidiBuffer&) { jassertfalse; }
void Processor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&) {
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Handle tempo-synced randomization and crossfading
    if (auto* playHead = getPlayHead()) {
        if (auto positionInfo = playHead->getPosition()) {
            double bpm = positionInfo->getBpm().orFallback(150.0);
            double ppqPosition = positionInfo->getPpqPosition().orFallback(0.0);
            
            // Update crossfade manager tempo
            verb.getCrossfadeManager().updateTempo(bpm, getSampleRate());
            
            // Process randomization
            verb.getRandomizer().processTempo(bpm, ppqPosition, verb);
            
            // Check if randomization has changed switches and update UI
            if (verb.getRandomizer().checkAndClearSwitchesChanged()) {
                // Capture the current switch states immediately
                BigInteger newCombs, newAllPasses;
                verb.getRandomizer().getUpdatedSwitchStates(verb, newCombs, newAllPasses);
                
                // Store for later UI update (safer than async)
                pendingCombsUpdate = newCombs.toString(2);
                pendingAllPassesUpdate = newAllPasses.toString(2);
                hasPendingUIUpdate = true;
            }
        }
    }

    if (buffer.getNumChannels() >= 2) {
        verb.processStereo (buffer.getWritePointer (0),
                            buffer.getWritePointer (1),
                            buffer.getNumSamples());
        rmsValue.set ((buffer.getRMSLevel (0, 0, buffer.getNumSamples()) + buffer.getRMSLevel (1, 0, buffer.getNumSamples())) * 0.5f);
    } else if (buffer.getNumChannels() == 1) {
        verb.processMono (buffer.getWritePointer (0),
                          buffer.getNumSamples());
        rmsValue.set (buffer.getRMSLevel (0, 0, buffer.getNumSamples()));
    }
}

bool Processor::hasEditor() const {
    return true;
}

AudioProcessorEditor* Processor::createEditor() {
    updateState();
    return new Editor (*this);
}

void Processor::getStateInformation (MemoryBlock& destData) {
    updateState();
    MemoryOutputStream stream (destData, false);
    state.writeToStream (stream);
}

void Processor::setStateInformation (const void* data, int sizeInBytes) {
    MemoryInputStream stream (data, (size_t) sizeInBytes, false);
    ValueTree nextState = ValueTree::readFromStream (stream);
    if (nextState.isValid())
        state.copyPropertiesFrom (nextState, nullptr);
}

void Processor::updateState() {
    state.removeListener (this);
    BigInteger combs, allpasses;
    {
        combs.setRange (0, 8, false);
        allpasses.setRange (0, 4, false);

        ScopedLock sl (getCallbackLock());
        params = verb.getParameters();
        verb.getEnablement (combs, allpasses);
    }

    state.setProperty (Tags::roomSize, params.roomSize, nullptr);
    state.setProperty (Tags::damping, params.damping, nullptr);
    state.setProperty (Tags::wetLevel, params.wetLevel, nullptr);
    state.setProperty (Tags::dryLevel, params.dryLevel, nullptr);
    state.setProperty (Tags::width, params.width, nullptr);
    state.setProperty (Tags::freezeMode, params.freezeMode, nullptr);
    state.setProperty (Tags::randomEnabled, params.randomEnabled, nullptr);
    state.setProperty (Tags::randomRate, params.randomRate, nullptr);
    state.setProperty (Tags::randomAmount, params.randomAmount, nullptr);
    state.setProperty (Tags::randomFilters, params.randomFilters, nullptr);
    state.setProperty (Tags::crossfadeRate, params.crossfadeRate, nullptr);
    state.setProperty (Tags::enabledAllPasses, allpasses.toString (2), nullptr);
    state.setProperty (Tags::enabledCombs, combs.toString (2), nullptr);
    state.addListener (this);
}

void Processor::valueTreePropertyChanged (ValueTree& tree, const Identifier& property) {
    const var& value (tree.getProperty (property));

    if (property == Tags::roomSize) {
        setParameter (SyncRoboVerb::RoomSize, (float) value);
    } else if (property == Tags::damping) {
        setParameter (SyncRoboVerb::Damping, (float) value);
    } else if (property == Tags::dryLevel) {
        setParameter (SyncRoboVerb::DryLevel, (float) value);
    } else if (property == Tags::wetLevel) {
        setParameter (SyncRoboVerb::WetLevel, (float) value);
    } else if (property == Tags::freezeMode) {
        setParameter (SyncRoboVerb::FreezeMode, (float) value);
    } else if (property == Tags::width) {
        setParameter (SyncRoboVerb::Width, (float) value);
    } else if (property == Tags::randomEnabled) {
        setParameter (SyncRoboVerb::RandomEnabled, (float) value);
    } else if (property == Tags::randomRate) {
        setParameter (SyncRoboVerb::RandomRate, (float) value);
    } else if (property == Tags::randomAmount) {
        setParameter (SyncRoboVerb::RandomAmount, (float) value);
    } else if (property == Tags::randomFilters) {
        setParameter (SyncRoboVerb::RandomFilters, (float) value);
    } else if (property == Tags::crossfadeRate) {
        setParameter (SyncRoboVerb::CrossfadeRate, (float) value);
    } else if (property == Tags::enabledCombs) {
        BigInteger enabled;
        enabled.parseString (value.toString(), 2);
        ScopedLock sl (getCallbackLock());
        verb.swapEnabledCombs (enabled);
    } else if (property == Tags::enabledAllPasses) {
        BigInteger enabled;
        enabled.parseString (value.toString(), 2);
        ScopedLock sl (getCallbackLock());
        verb.swapEnabledAllPasses (enabled);
    }
}

void Processor::processPendingUIUpdates() {
    if (hasPendingUIUpdate.get()) {
        state.setProperty(Tags::enabledCombs, pendingCombsUpdate, nullptr);
        state.setProperty(Tags::enabledAllPasses, pendingAllPassesUpdate, nullptr);
        hasPendingUIUpdate = false;
    }
}
} // namespace syncroboverb

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new syncroboverb::Processor();
}

#if 0
    #include <juce_audio_processors/juce_audio_processors.h>
    #include "syncroboverb.hpp"
    #include "pluginview.hpp"

using namespace juce;

namespace syncroboverb {

namespace detail {

static float valueGet (int paramIndex, const SyncRoboVerb::Parameters& ps) {
    // clang-format off
    switch (paramIndex) {
        case SyncRoboVerb::RoomSize: return ps.roomSize; break;
        case SyncRoboVerb::Damping:  return ps.damping;  break;
        case SyncRoboVerb::WetLevel: return ps.wetLevel; break;
        case SyncRoboVerb::DryLevel: return ps.dryLevel; break;
        case SyncRoboVerb::Width:    return ps.width;    break;
    }
    // clang-format on
    return 0.0f;
}

static void valueAssign (int paramIndex, SyncRoboVerb::Parameters& ps, float value) {
    // clang-format off
    switch (paramIndex) {
        case SyncRoboVerb::RoomSize: ps.roomSize = value; break;
        case SyncRoboVerb::Damping:  ps.damping  = value; break;
        case SyncRoboVerb::WetLevel: ps.wetLevel = value; break;
        case SyncRoboVerb::DryLevel: ps.dryLevel = value; break;
        case SyncRoboVerb::Width:    ps.width    = value; break;
    }
    // clang-format on
}

} // namespace detail

class Editor : public juce::AudioProcessorEditor {
public:
    Editor (AudioProcessor& p) : juce::AudioProcessorEditor (p) {
        addAndMakeVisible (_view);
        setSize (_view.getWidth(), _view.getHeight());
    }

    ~Editor() {}

    void resized() override {
        _view.setTopLeftPosition (0, 0);
    }

    void paint (juce::Graphics& g) override {
        g.fillAll (juce::Colours::black);
    }

private:
    PluginView _view;
};

class Processor : public juce::AudioProcessor,
                  public juce::ValueTree::Listener {
    SyncRoboVerb _verb;
    juce::ValueTree _state { "SyncRoboVerb " };

public:
    Processor() : juce::AudioProcessor() {
        SyncRoboVerb::Parameters defaults;

        // note: do not change the order of parameters.
        addParameter (new AudioParameterFloat ("roomSize", "Room Size", 0.0f, 1.0f, defaults.roomSize));
        addParameter (new AudioParameterFloat ("damping", "Damping", 0.0f, 1.0f, defaults.damping));
        addParameter (new AudioParameterFloat ("wetLevel", "Wet Level", 0.0f, 1.0f, defaults.wetLevel));
        addParameter (new AudioParameterFloat ("dryLevel", "Dry Level", 0.0f, 1.0f, defaults.dryLevel));
        addParameter (new AudioParameterFloat ("width", "Width", 0.0f, 1.0f, defaults.width));

        for (int i = 0; i < SyncRoboVerb::numCombs; ++i) {
            new AudioParameterBool (String ("comb-") + String (i + 1),
                                    String ("Comb ") + String (i + 1),
                                    ! juce::exactlyEqual (0.0f, _verb.toggledCombFloat (i)));
        }

        for (int i = 0; i < SyncRoboVerb::numAllPasses; ++i) {
            new AudioParameterBool (String ("allpass-") + String (i + 1),
                                    String ("All Pass ") + String (i + 1),
                                    ! juce::exactlyEqual (0.0f, _verb.toggledAllPassFloat (i)));
        }
    }

    ~Processor() override {}

    //==========================================================================
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&,
                                int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    void valueTreeRedirected (ValueTree&) override {}
    
    SyncRoboVerb::Parameters safeParams() const noexcept {
        juce::ScopedLock sl (getCallbackLock());
        return _verb.getParameters();
    }

    void valueTreePropertyChanged (ValueTree& tree, const Identifier& property) {
        const var& value (tree.getProperty (property));
        auto rparams = safeParams();

        bool changed = true;
        auto idx = SyncRoboVerb::RoomSize;
        if (property == Tags::roomSize) {
            idx = SyncRoboVerb::RoomSize;
    
        } else if (property == Tags::damping) {
           idx = SyncRoboVerb::Damping
        } else if (property == Tags::dryLevel) {
            idx = SyncRoboVerb::DryLevel;
        } else if (property == Tags::wetLevel) {
            idx = SyncRoboVerb::WetLevel;
        } else if (property == Tags::freezeMode) {
            idx = SyncRoboVerb::FreezeMode;
        } else if (property == Tags::width) {
            idx = SyncRoboVerb::Width;
        } else {
            changed = false;
        }

        if (changed) {
            float v2 = (float) value;
            ScopedLock sl (getCallbackLock());
            getParameters().getUnchecked(idx)->setValueNotifyingHost (v2);
            return;
        }
        
        if (property == Tags::enabledCombs) {
            BigInteger enabled;
            enabled.parseString (value.toString(), 2);
            ScopedLock sl (getCallbackLock());
            _verb.swapEnabledCombs (enabled);
        } else if (property == Tags::enabledAllPasses) {
            BigInteger enabled;
            enabled.parseString (value.toString(), 2);
            ScopedLock sl (getCallbackLock());
            _verb.swapEnabledAllPasses (enabled);
        }
    }

    void updateState() {
        SyncRoboVerb::Parameters rparams;
        _state.removeListener (this);
        BigInteger combs, allpasses;
        {
            combs.setRange (0, 8, false);
            allpasses.setRange (0, 4, false);

            ScopedLock sl (getCallbackLock());
            rparams = _verb.getParameters();
            _verb.getEnablement (combs, allpasses);
        }

        _state.setProperty (Tags::roomSize, rparams.roomSize, nullptr);
        _state.setProperty (Tags::damping, rparams.damping, nullptr);
        _state.setProperty (Tags::wetLevel, rparams.wetLevel, nullptr);
        _state.setProperty (Tags::dryLevel, rparams.dryLevel, nullptr);
        _state.setProperty (Tags::width, rparams.width, nullptr);
        _state.setProperty (Tags::freezeMode, rparams.freezeMode, nullptr);
        _state.setProperty (Tags::enabledAllPasses, allpasses.toString (2), nullptr);
        _state.setProperty (Tags::enabledCombs, combs.toString (2), nullptr);
        _state.addListener (this);
    }

    //==============================================================================
    const String getName() const override { return "SyncRoboVerb"; }
    StringArray getAlternateDisplayNames() const override { return { "RBVB" }; }

    //==============================================================================
    void prepareToPlay (double sampleRate, int) override {
        _verb.setSampleRate (sampleRate);
        _verb.reset();
    }

    void releaseResources() override {}

    void memoryWarningReceived() override { jassertfalse; }

    void processParameters() {
        auto& params (getParameters());
        auto rparams (_verb.getParameters());

        int numParamChanges = 0;
        for (int i = 0; i < SyncRoboVerb::numParameters; ++i) {
            auto param = dynamic_cast<AudioParameterFloat*> (params.getUnchecked (i));
            auto val = param->get();
            if (juce::exactlyEqual (val, detail::valueGet (i, rparams)))
                continue;

            ++numParamChanges;
            detail::valueAssign (i, rparams, val);
        }

        if (numParamChanges > 0)
            _verb.setParameters (rparams);

        for (int i = 0; i < SyncRoboVerb::numCombs; ++i) {
            auto isOn = dynamic_cast<AudioParameterBool*> (
                params.getUnchecked (i + SyncRoboVerb::numParameters));
            _verb.setCombToggle (i, *isOn);
        }

        for (int i = 0; i < SyncRoboVerb::numAllPasses; ++i) {
            auto isOn = dynamic_cast<AudioParameterBool*> (
                params.getUnchecked (i + SyncRoboVerb::numParameters + SyncRoboVerb::numCombs));
            _verb.setAllPassToggle (i, *isOn);
        }
    }

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override {
        processParameters();
        _verb.processStereo (buffer.getWritePointer (0),
                             buffer.getWritePointer (1),
                             buffer.getWritePointer (0),
                             buffer.getWritePointer (1),
                             buffer.getNumSamples());
    }

    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override {
        AudioProcessor::processBlock (buffer, midiMessages);
    }
    #if 0
    
    // virtual void processBlockBypassed (AudioBuffer<float>& buffer, MidiBuffer& midiMessages);
    // virtual void processBlockBypassed (AudioBuffer<double>& buffer, MidiBuffer& midiMessages);
    #endif

    #if 0
    //==============================================================================
    virtual bool canAddBus (bool isInput) const;
    virtual bool canRemoveBus (bool isInput) const;
    #endif
    //==============================================================================
    // virtual bool supportsDoublePrecisionProcessing() const;

    //==============================================================================
    double getTailLengthSeconds() const override { return 0.0; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }

    //==============================================================================
    void reset() override { _verb.reset(); }

    //==============================================================================
    bool hasEditor() const override { return true; }
    AudioProcessorEditor* createEditor() override {
        return new Editor (*this);
    }

    //==============================================================================
    int getNumPrograms() override { return 1; }

    /** Returns the number of the currently active program. */
    int getCurrentProgram() override { return 0; }

    /** Called by the host to change the current program. */
    void setCurrentProgram (int index) override { juce::ignoreUnused (index); }

    /** Must return the name of a given program. */
    const String getProgramName (int index) override {
        juce::ignoreUnused (index);
        return "Default";
    }

    /** Called by the host to rename a program. */
    void changeProgramName (int index, const String& newName) override {
        juce::ignoreUnused (index, newName);
    }

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override {
        juce::ignoreUnused (destData);
    }

    void setStateInformation (const void* data, int sizeInBytes) override {
        juce::ignoreUnused (data, sizeInBytes);
    }

    #if 0
    /** The host will call this method if it wants to save the state of just the processor's
        current program.

        Unlike getStateInformation, this should only return the current program's state.

        Not all hosts support this, and if you don't implement it, the base class
        method just calls getStateInformation() instead. If you do implement it, be
        sure to also implement getCurrentProgramStateInformation.

        @see getStateInformation, setCurrentProgramStateInformation
    */
    virtual void getCurrentProgramStateInformation (juce::MemoryBlock& destData);

    /** The host will call this method if it wants to restore the state of just the processor's
        current program.

        Not all hosts support this, and if you don't implement it, the base class
        method just calls setStateInformation() instead. If you do implement it, be
        sure to also implement getCurrentProgramStateInformation.

        @see setStateInformation, getCurrentProgramStateInformation
    */
    virtual void setCurrentProgramStateInformation (const void* data, int sizeInBytes);

    /** This method is called when the total number of input or output channels is changed. */
    virtual void numChannelsChanged();

    /** This method is called when the number of buses is changed. */
    virtual void numBusesChanged();

    /** This method is called when the layout of the audio processor changes. */
    virtual void processorLayoutsChanged();

    //==============================================================================
    /** Adds a listener that will be called when an aspect of this processor changes. */
    virtual void addListener (AudioProcessorListener* newListener);

    /** Removes a previously added listener. */
    virtual void removeListener (AudioProcessorListener* listenerToRemove);

    //==============================================================================
    /** Tells the processor to use this playhead object.
        The processor will not take ownership of the object, so the caller must delete it when
        it is no longer being used.
    */
    virtual void setPlayHead (AudioPlayHead* newPlayHead);

    //==============================================================================
    /** AAX plug-ins need to report a unique "plug-in id" for every audio layout
        configuration that your AudioProcessor supports on the main bus. Override this
        function if you want your AudioProcessor to use a custom "plug-in id" (for example
        to stay backward compatible with older versions of JUCE).

        The default implementation will compute a unique integer from the input and output
        layout and add this value to the 4 character code 'jcaa' (for native AAX) or 'jyaa'
        (for AudioSuite plug-ins).
    */
    virtual int32 getAAXPluginIDForMainBusConfig (const AudioChannelSet& mainInputLayout,
                                                  const AudioChannelSet& mainOutputLayout,
                                                  bool idForAudioSuite) const;

    virtual CurveData getResponseCurve (CurveData::Type /*curveType*/) const      { return {}; }

    /** Informs the AudioProcessor that track properties such as the track's name or
        colour has been changed.

        If you are hosting this AudioProcessor then use this method to inform the
        AudioProcessor about which track the AudioProcessor is loaded on. This method
        may only be called on the message thread.

        If you are implementing an AudioProcessor then you can override this callback
        to do something useful with the track properties such as changing the colour
        of your AudioProcessor's editor. It's entirely up to the host when and how
        often this callback will be called.

        The default implementation of this callback will do nothing.
    */
    virtual void updateTrackProperties (const TrackProperties& properties);
    #endif

protected:
    #if 0
    /** Callback to query if the AudioProcessor supports a specific layout.

        This callback is called when the host probes the supported bus layouts via
        the checkBusesLayoutSupported method. You should override this callback if you
        would like to limit the layouts that your AudioProcessor supports. The default
        implementation will accept any layout. JUCE does basic sanity checks so that
        the provided layouts parameter will have the same number of buses as your
        AudioProcessor.

        @see checkBusesLayoutSupported
    */
    virtual bool isBusesLayoutSupported (const BusesLayout&) const          { return true; }

    /** Callback to check if a certain bus layout can now be applied.

        Most subclasses will not need to override this method and should instead
        override the isBusesLayoutSupported callback to reject certain layout changes.

        This callback is called when the user requests a layout change. It will only be
        called if processing of the AudioProcessor has been stopped by a previous call to
        releaseResources or after the construction of the AudioProcessor. It will be called
        just before the actual layout change. By returning false you will abort the layout
        change and setBusesLayout will return false indicating that the layout change
        was not successful.

        The default implementation will simply call isBusesLayoutSupported.

        You only need to override this method if there is a chance that your AudioProcessor
        may not accept a layout although you have previously claimed to support it via the
        isBusesLayoutSupported callback. This can occur if your AudioProcessor's supported
        layouts depend on other plug-in parameters which may have changed since the last
        call to isBusesLayoutSupported, such as the format of an audio file which can be
        selected by the user in the AudioProcessor's editor. This callback gives the
        AudioProcessor a last chance to reject a layout if conditions have changed as it
        is always called just before the actual layout change.

        As it is never called while the AudioProcessor is processing audio, it can also
        be used for AudioProcessors which wrap other plug-in formats to apply the current
        layout to the underlying plug-in. This callback gives such AudioProcessors a
        chance to reject the layout change should an error occur with the underlying plug-in
        during the layout change.

        @see isBusesLayoutSupported, setBusesLayout
    */
    virtual bool canApplyBusesLayout (const BusesLayout& layouts) const     { return isBusesLayoutSupported (layouts); }

    /** This method will be called when a new bus layout needs to be applied.

        Most subclasses will not need to override this method and should just use the default
        implementation.
    */
    virtual bool applyBusLayouts (const BusesLayout& layouts);

    /** Callback to query if adding/removing buses currently possible.

        This callback is called when the host calls addBus or removeBus.
        Similar to canApplyBusesLayout, this callback is only called while
        the AudioProcessor is stopped and gives the processor a last
        chance to reject a requested bus change. It can also be used to apply
        the bus count change to an underlying wrapped plug-in.

        When adding a bus, isAddingBuses will be true and the plug-in is
        expected to fill out outNewBusProperties with the properties of the
        bus which will be created just after the successful return of this callback.

        Implementations of AudioProcessor will rarely need to override this
        method. Only override this method if your processor supports adding
        and removing buses and if it needs more fine grain control over the
        naming of new buses or may reject bus number changes although canAddBus
        or canRemoveBus returned true.

        The default implementation will return false if canAddBus/canRemoveBus
        returns false (the default behavior). Otherwise, this method returns
        "Input #busIndex" for input buses and "Output #busIndex" for output buses
        where busIndex is the index for newly created buses. The default layout
        in this case will be the layout of the previous bus of the same direction.
    */
    virtual bool canApplyBusCountChange (bool isInput, bool isAddingBuses,
                                         BusProperties& outNewBusProperties);
    #endif
};

} // namespace syncroboverb


JUCE_PUBLIC_FUNCTION
juce::AudioProcessor* createPluginFilter() {
    return new syncroboverb::Processor();
}
#endif
