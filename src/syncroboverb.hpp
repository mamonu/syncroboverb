// Copyright (C) 2015-2025  Kushview, LLC <info@kushview.net>
// SPDX-License-Identifier: GPL3-or-later

#pragma once

#include <cmath>
#include <cstring>
#include <memory>

#include <juce_core/juce_core.h>

using juce::BigInteger;
using juce::Identifier;

namespace Tags {
static const Identifier roomSize = "roomSize";
static const Identifier damping = "damping";
static const Identifier wetLevel = "wetLevel";
static const Identifier dryLevel = "dryLevel";
static const Identifier width = "width";
static const Identifier freezeMode = "freezeMode";
static const Identifier enabledCombs = "enabledCombs";
static const Identifier enabledAllPasses = "enabledAllPasses";
static const Identifier randomEnabled = "randomEnabled";
static const Identifier randomRate = "randomRate";
static const Identifier randomAmount = "randomAmount";
static const Identifier randomFilters = "randomFilters";
static const Identifier crossfadeRate = "crossfadeRate";
static const Identifier allPassGain1 = "allPassGain1";
static const Identifier allPassGain2 = "allPassGain2";
static const Identifier allPassGain3 = "allPassGain3";
static const Identifier allPassGain4 = "allPassGain4";
}; // namespace Tags

class TempoSyncedRandomizer {
public:
    enum RandomRate {
        SixteenthNote = 0,
        EighthNote,
        QuarterNote,
        HalfNote,
        WholeNote,
        TwoBars,
        FourBars,
        EightBars,
        numRates
    };
    
    enum FilterType {
        CombsOnly = 0,
        AllPassOnly,
        Both,
        numFilterTypes
    };
    
    TempoSyncedRandomizer() : rng(juce::Random::getSystemRandom().nextInt64()) {
        reset();
    }
    
    void reset() {
        lastPpqPosition = 0.0;
        enabled = false;
        rate = QuarterNote;
        amount = 0.5f;
        filterType = Both;
        switchesChanged = false;
    }
    
    void setEnabled(bool shouldBeEnabled) { 
        enabled = shouldBeEnabled; 
        if (enabled) lastPpqPosition = 0.0; // Reset timing when enabled
    }
    void setRate(RandomRate newRate) { 
        rate = newRate; 
        lastPpqPosition = 0.0; // Reset timing when rate changes
    }
    void setAmount(float newAmount) { amount = juce::jlimit(0.0f, 1.0f, newAmount); }
    void setFilterType(FilterType newType) { 
        filterType = newType; 
        lastPpqPosition = 0.0; // Reset timing when filter type changes
    }
    
    bool isEnabled() const { return enabled; }
    RandomRate getRate() const { return rate; }
    float getAmount() const { return amount; }
    FilterType getFilterType() const { return filterType; }
    
    void processTempo(double bpm, double ppqPosition, class SyncRoboVerb& verb);
    
    // Get the updated switch states after randomization for UI updates
    void getUpdatedSwitchStates(class SyncRoboVerb& verb, BigInteger& combs, BigInteger& allpasses);
    
    // Check if switches have changed and clear the flag
    bool checkAndClearSwitchesChanged() {
        bool changed = switchesChanged;
        switchesChanged = false;
        return changed;
    }
    
private:
    bool enabled;
    RandomRate rate;
    float amount;
    FilterType filterType;
    double lastPpqPosition;
    juce::Random rng;
    bool switchesChanged;
    
    double getRateInQuarterNotes() const {
        switch (rate) {
            case SixteenthNote: return 0.25;
            case EighthNote: return 0.5;
            case QuarterNote: return 1.0;
            case HalfNote: return 2.0;
            case WholeNote: return 4.0;
            case TwoBars: return 8.0;
            case FourBars: return 16.0;
            case EightBars: return 32.0;
            default: return 1.0;
        }
    }
    
    void randomizeSwitches(class SyncRoboVerb& verb);
};

class TempoSyncedCrossfadeManager {
public:
    enum CrossfadeTiming {
        Immediate = 0,      // ~1ms (emergency/click protection only)
        Sixtyfourth,        // 1/64 note
        Thirtysecond,       // 1/32 note  
        Sixteenth,          // 1/16 note
        Eighth,             // 1/8 note
        Quarter,            // 1/4 note
        numTimings
    };
    
    TempoSyncedCrossfadeManager() : crossfadeTiming(Thirtysecond), currentBPM(150.0), sampleRate(44100.0) {}
    
    void updateTempo(double bpm, double sr) {
        currentBPM = bpm;
        sampleRate = sr;
    }
    
    void setCrossfadeTiming(CrossfadeTiming timing) {
        crossfadeTiming = timing;
    }
    
    CrossfadeTiming getCrossfadeTiming() const { return crossfadeTiming; }
    
    int calculateFadeSamples() const {
        if (crossfadeTiming == Immediate) {
            return (int)(sampleRate * 0.001);  // 1ms click protection
        }
        
        double beatsPerSample = currentBPM / (60.0 * sampleRate);
        double fadeDurationBeats = getNoteDuration(crossfadeTiming);
        return (int)(fadeDurationBeats / beatsPerSample);
    }
    
private:
    CrossfadeTiming crossfadeTiming;
    double currentBPM;
    double sampleRate;
    
    double getNoteDuration(CrossfadeTiming timing) const {
        switch (timing) {
            case Sixtyfourth: return 0.0625;   // 1/64 note
            case Thirtysecond: return 0.125;   // 1/32 note  
            case Sixteenth: return 0.25;       // 1/16 note
            case Eighth: return 0.5;           // 1/8 note
            case Quarter: return 1.0;          // 1/4 note
            default: return 0.125;
        }
    }
};

class SyncRoboVerb {
public:
    enum ParameterIndex {
        RoomSize = 0,
        Damping,
        WetLevel,
        DryLevel,
        Width,
        FreezeMode,
        RandomEnabled,
        RandomRate,
        RandomAmount,
        RandomFilters,
        CrossfadeRate,
        AllPassGain1,
        AllPassGain2,
        AllPassGain3,
        AllPassGain4,
        numParameters,
        numCombs = 8,
        numAllPasses = 4,
        numChannels = 2
    };

    SyncRoboVerb() {
        for (int i = 0; i < numCombs; ++i)
            enabledCombs[i] = false;
        enabledCombs[3] = true;
        enabledCombs[4] = true;
        enabledCombs[5] = true;

        for (int i = 0; i < numAllPasses; ++i)
            enabledAllPasses[i] = false;
        enabledAllPasses[0] = true;
        enabledAllPasses[1] = true;
        
        // Initialize AllPass gains (default to full gain = current behavior)
        for (int i = 0; i < numAllPasses; ++i)
            allPassGains[i] = 1.0f;

        setParameters (Parameters());
        setSampleRate (44100.0);
    }

    /** Holds the parameters being used by a Reverb object. */
    struct Parameters {
        Parameters() noexcept
            : roomSize (0.5f),
              damping (0.5f),
              wetLevel (0.33f),
              dryLevel (0.4f),
              width (1.0f),
              freezeMode (0),
              randomEnabled (0.0f),
              randomRate (2.0f),
              randomAmount (0.5f),
              randomFilters (0.0f),  // CombsOnly
              crossfadeRate (2.0f),
              allPassGain1 (1.0f),
              allPassGain2 (1.0f),
              allPassGain3 (1.0f),
              allPassGain4 (1.0f) {}

        float roomSize;   /**< Room size, 0 to 1.0, where 1.0 is big, 0 is small. */
        float damping;    /**< Damping, 0 to 1.0, where 0 is not damped, 1.0 is fully damped. */
        float wetLevel;   /**< Wet level, 0 to 1.0 */
        float dryLevel;   /**< Dry level, 0 to 1.0 */
        float width;      /**< Reverb width, 0 to 1.0, where 1.0 is very wide. */
        float freezeMode; /**< Freeze mode - values < 0.5 are "normal" mode, values > 0.5
                             put the reverb into a continuous feedback loop. */
        float randomEnabled; /**< Random switching enabled, 0 to 1.0 */
        float randomRate;    /**< Random switching rate, 0 to numRates-1 */
        float randomAmount;  /**< Random switching probability, 0 to 1.0 */
        float randomFilters; /**< Which filters to randomize, 0 to numFilterTypes-1 */
        float crossfadeRate; /**< Crossfade timing, 0 to numTimings-1 */
        float allPassGain1;  /**< AllPass filter 1 gain, 0 to 1.0 */
        float allPassGain2;  /**< AllPass filter 2 gain, 0 to 1.0 */
        float allPassGain3;  /**< AllPass filter 3 gain, 0 to 1.0 */
        float allPassGain4;  /**< AllPass filter 4 gain, 0 to 1.0 */

        Parameters (const Parameters& o) { operator= (o); }
        Parameters& operator= (const Parameters& o) {
            roomSize = o.roomSize;
            damping = o.damping;
            wetLevel = o.wetLevel;
            dryLevel = o.dryLevel;
            width = o.width;
            freezeMode = o.freezeMode;
            randomEnabled = o.randomEnabled;
            randomRate = o.randomRate;
            randomAmount = o.randomAmount;
            randomFilters = o.randomFilters;
            crossfadeRate = o.crossfadeRate;
            allPassGain1 = o.allPassGain1;
            allPassGain2 = o.allPassGain2;
            allPassGain3 = o.allPassGain3;
            allPassGain4 = o.allPassGain4;
            return *this;
        }

        bool operator== (const Parameters& o) {
            // clang-format off
            return (juce::exactlyEqual (roomSize, o.roomSize) && 
                    juce::exactlyEqual (damping, o.damping) && 
                    juce::exactlyEqual (wetLevel , o.wetLevel) && 
                    juce::exactlyEqual (dryLevel , o.dryLevel) && 
                    juce::exactlyEqual (width , o.width) && 
                    juce::exactlyEqual (freezeMode , o.freezeMode) &&
                    juce::exactlyEqual (randomEnabled , o.randomEnabled) &&
                    juce::exactlyEqual (randomRate , o.randomRate) &&
                    juce::exactlyEqual (randomAmount , o.randomAmount) &&
                    juce::exactlyEqual (randomFilters , o.randomFilters) &&
                    juce::exactlyEqual (crossfadeRate , o.crossfadeRate) &&
                    juce::exactlyEqual (allPassGain1 , o.allPassGain1) &&
                    juce::exactlyEqual (allPassGain2 , o.allPassGain2) &&
                    juce::exactlyEqual (allPassGain3 , o.allPassGain3) &&
                    juce::exactlyEqual (allPassGain4 , o.allPassGain4));
            // clang-format on
        }

        bool operator!= (const Parameters& o) { return ! operator== (o); }
    };

    const Parameters& getParameters() const noexcept { return parameters; }

    void swapEnabledCombs (BigInteger& e) {
        for (int i = 0; i < numCombs; ++i)
            enabledCombs[i] = e[i];
    }

    void swapEnabledAllPasses (BigInteger& e) {
        for (int i = 0; i < numAllPasses; ++i)
            enabledAllPasses[i] = e[i];
    }

    void getEnablement (BigInteger& c, BigInteger& a) const {
        for (int i = 0; i < numCombs; ++i)
            c.setBit (i, enabledCombs[i]);
        for (int i = 0; i < numAllPasses; ++i)
            a.setBit (i, enabledAllPasses[i]);
    }

    void setCombToggle (const int index, const bool toggled) {
        enabledCombs[index] = toggled;
    }

    void setAllPassToggle (const int index, const bool toggled) {
        enabledAllPasses[index] = toggled;
    }

    float toggledCombFloat (const int index) const {
        return enabledCombs[index] ? 1.0f : 0.0f;
    }

    float toggledAllPassFloat (const int index) const {
        return enabledAllPasses[index] ? 1.0f : 0.0f;
    }
    
    void setAllPassGain (const int index, const float gain) {
        jassert (index >= 0 && index < numAllPasses);
        allPassGains[index] = juce::jlimit (0.0f, 1.0f, gain);
    }
    
    float getAllPassGain (const int index) const {
        jassert (index >= 0 && index < numAllPasses);
        return allPassGains[index];
    }

    void setParameters (const Parameters& newParams) {
        const float wetScaleFactor = 6.0f;
        const float dryScaleFactor = 2.0f;

        const float wet = newParams.wetLevel * wetScaleFactor;
        dryGain.setValue (newParams.dryLevel * dryScaleFactor);
        wetGain1.setValue (0.5f * wet * (1.0f + newParams.width));
        wetGain2.setValue (0.5f * wet * (1.0f - newParams.width));

        gain = isFrozen (newParams.freezeMode) ? 0.0f : 0.015f;
        
        // Update AllPass gains from parameters
        allPassGains[0] = newParams.allPassGain1;
        allPassGains[1] = newParams.allPassGain2;
        allPassGains[2] = newParams.allPassGain3;
        allPassGains[3] = newParams.allPassGain4;
        
        parameters = newParams;
        updateDamping();
    }

    void setSampleRate (const double sampleRate) {
        //static const short combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
        static const short combTunings[] = { 8092, 4096, 2048, 1024, 512, 256, 128, 64 }; // (at 44100Hz)
        static const short allPassTunings[] = { 556, 441, 341, 225 };
        const int stereoSpread = 23;
        const int intSampleRate = (int) sampleRate;

        for (int i = 0; i < numCombs; ++i) {
            comb[0][i].setSize ((intSampleRate * combTunings[i]) / 44100);
            comb[1][i].setSize ((intSampleRate * (combTunings[i] + stereoSpread)) / 44100);
        }

        for (int i = 0; i < numAllPasses; ++i) {
            allPass[0][i].setSize ((intSampleRate * allPassTunings[i]) / 44100);
            allPass[1][i].setSize ((intSampleRate * (allPassTunings[i] + stereoSpread)) / 44100);
        }

        const double smoothTime = 0.01;
        damping.reset (sampleRate, smoothTime);
        feedback.reset (sampleRate, smoothTime);
        dryGain.reset (sampleRate, smoothTime);
        wetGain1.reset (sampleRate, smoothTime);
        wetGain2.reset (sampleRate, smoothTime);
    }

    /** Clears the reverb's buffers. */
    void reset() {
        for (int j = 0; j < numChannels; ++j) {
            for (int i = 0; i < numCombs; ++i)
                comb[j][i].clear();

            for (int i = 0; i < numAllPasses; ++i)
                allPass[j][i].clear();
        }
    }

    void processStereo (float* const left, float* const right, const int numSamples) noexcept
    {
        jassert (left != nullptr && right != nullptr);

        for (int i = 0; i < numSamples; ++i)
        {
            const float input = (left[i] + right[i]) * gain;
            float outL = 0, outR = 0;

            const float damp    = damping.getNextValue();
            const float feedbck = feedback.getNextValue();

            for (int j = 0; j < numCombs; ++j)  // accumulate the comb filters in parallel
            {
                if (! enabledCombs [j])
                    continue;
                outL += comb[0][j].process (input, damp, feedbck);
                outR += comb[1][j].process (input, damp, feedbck);
            }

            for (int j = 0; j < numAllPasses; ++j)  // run the allpass filters in series
            {
                if (allPassGains[j] > 0.0f) {
                    float processedL = allPass[0][j].process (outL);
                    float processedR = allPass[1][j].process (outR);
                    
                    // Mix processed and dry signal based on gain
                    outL = outL * (1.0f - allPassGains[j]) + processedL * allPassGains[j];
                    outR = outR * (1.0f - allPassGains[j]) + processedR * allPassGains[j];
                }
            }

            const float dry  = dryGain.getNextValue();
            const float wet1 = wetGain1.getNextValue();
            const float wet2 = wetGain2.getNextValue();

            left[i]  = outL * wet1 + outR * wet2 + left[i]  * dry;
            right[i] = outR * wet1 + outL * wet2 + right[i] * dry;
        }
    }

    /** Applies the reverb to a single mono channel of audio data. */
    void processMono (float* const samples, const int numSamples) noexcept
    {
        jassert (samples != nullptr);

        for (int i = 0; i < numSamples; ++i)
        {
            const float input = samples[i] * gain;
            float output = 0;

            const float damp    = damping.getNextValue();
            const float feedbck = feedback.getNextValue();

            for (int j = 0; j < numCombs; ++j)
            {
                // accumulate the comb filters in parallel
                if (! enabledCombs [j])
                    continue;
                output += comb[0][j].process (input, damp, feedbck);
            }

            for (int j = 0; j < numAllPasses; ++j)
            {
                // run the allpass filters in series with gain control
                if (allPassGains[j] > 0.0f) {
                    float processed = allPass[0][j].process (output);
                    
                    // Mix processed and dry signal based on gain
                    output = output * (1.0f - allPassGains[j]) + processed * allPassGains[j];
                }
            }

            const float dry  = dryGain.getNextValue();
            const float wet1 = wetGain1.getNextValue();

            samples[i] = output * wet1 + samples[i] * dry;
        }
    }

private:
    static bool isFrozen (const float freezeMode) noexcept { return freezeMode >= 0.5f; }

    void updateDamping() noexcept {
        const float roomScaleFactor = 0.28f;
        const float roomOffset = 0.7f;
        const float dampScaleFactor = 0.4f;

        if (isFrozen (parameters.freezeMode))
            setDamping (0.0f, 1.0f);
        else
            setDamping (parameters.damping * dampScaleFactor,
                        parameters.roomSize * roomScaleFactor + roomOffset);
    }

    void setDamping (const float dampingToUse, const float roomSizeToUse) noexcept {
        damping.setValue (dampingToUse);
        feedback.setValue (roomSizeToUse);
    }

    class CombFilter {
    public:
        CombFilter() noexcept : bufferSize (0), bufferIndex (0), last (0), 
                               targetGain(1.0f), currentGain(0.0f), 
                               fadeSamplesRemaining(0), totalFadeSamples(0) {}

        void setSize (const int size) {
            if ((size_t) size != bufferSize) {
                bufferIndex = 0;
                bufferSize = (size_t) size;
                buffer.reset (new float[bufferSize]);
            }

            clear();
        }

        void clear() noexcept {
            last = 0;
            memset (buffer.get(), 0, sizeof (float) * (size_t) bufferSize);
        }
        
        void startFade(bool enabled, int fadeDurationSamples) noexcept {
            targetGain = enabled ? 1.0f : 0.0f;
            totalFadeSamples = fadeDurationSamples;
            fadeSamplesRemaining = fadeDurationSamples;
        }

        float process (const float input, const float damp, const float feedbackLevel) noexcept {
            // Update crossfade if active
            if (fadeSamplesRemaining > 0) {
                float progress = 1.0f - (float(fadeSamplesRemaining) / totalFadeSamples);
                currentGain = currentGain + (targetGain - currentGain) * progress;
                fadeSamplesRemaining--;
            } else {
                currentGain = targetGain;
            }
            
            const float output = buffer[bufferIndex];
            last = (output * (1.0f - damp)) + (last * damp);
            JUCE_UNDENORMALISE (last);

            float temp = input + (last * feedbackLevel);
            JUCE_UNDENORMALISE (temp);
            buffer[bufferIndex] = temp;
            bufferIndex = (bufferIndex + 1) % bufferSize;
            return output * currentGain;
        }

    private:
        std::unique_ptr<float[]> buffer;
        std::size_t bufferSize { 0 };
        std::size_t bufferIndex { 0 };
        float last;
        
        // Crossfade support
        float targetGain;
        float currentGain;
        int fadeSamplesRemaining;
        int totalFadeSamples;
    };

    //==============================================================================
    class AllPassFilter {
    public:
        AllPassFilter() noexcept : bufferSize (0), bufferIndex (0), 
                                  targetGain(1.0f), currentGain(0.0f), 
                                  fadeSamplesRemaining(0), totalFadeSamples(0) {}

        void setSize (const int size) {
            if ((size_t) size != bufferSize) {
                bufferIndex = 0;
                buffer.reset (new float[(size_t) size]);
                bufferSize = (size_t) size;
            }

            clear();
        }

        void clear() noexcept {
            memset (buffer.get(), 0, sizeof (float) * (size_t) bufferSize);
        }
        
        void startFade(bool enabled, int fadeDurationSamples) noexcept {
            targetGain = enabled ? 1.0f : 0.0f;
            totalFadeSamples = fadeDurationSamples;
            fadeSamplesRemaining = fadeDurationSamples;
        }

        float process (const float input) noexcept {
            // Update crossfade if active
            if (fadeSamplesRemaining > 0) {
                float progress = 1.0f - (float(fadeSamplesRemaining) / totalFadeSamples);
                currentGain = currentGain + (targetGain - currentGain) * progress;
                fadeSamplesRemaining--;
            } else {
                currentGain = targetGain;
            }
            
            const float bufferedValue = buffer[bufferIndex];
            float temp = input + (bufferedValue * 0.5f);
            // JUCE_UNDENORMALISE (temp);
            buffer[bufferIndex] = temp;
            bufferIndex = (bufferIndex + 1) % bufferSize;
            float output = bufferedValue - input;
            return output * currentGain;
        }

    private:
        std::unique_ptr<float[]> buffer;
        std::size_t bufferSize, bufferIndex;
        
        // Crossfade support
        float targetGain;
        float currentGain;
        int fadeSamplesRemaining;
        int totalFadeSamples;
    };

    class LinearSmoothedValue {
    public:
        LinearSmoothedValue() noexcept
            : currentValue (0), target (0), step (0), countdown (0), stepsToTarget (0) {}

        void reset (double sampleRate, double fadeLengthSeconds) noexcept {
            // jassert (sampleRate > 0 && fadeLengthSeconds >= 0);
            stepsToTarget = (int) std::floor (fadeLengthSeconds * sampleRate);
            currentValue = target;
            countdown = 0;
        }

        void setValue (float newValue) noexcept {
            if (! juce::exactlyEqual (target, newValue)) {
                target = newValue;
                countdown = stepsToTarget;

                if (countdown <= 0)
                    currentValue = target;
                else
                    step = (target - currentValue) / (float) countdown;
            }
        }

        float getNextValue() noexcept {
            if (countdown <= 0)
                return target;

            --countdown;
            currentValue += step;
            return currentValue;
        }

    private:
        float currentValue, target, step;
        int countdown, stepsToTarget;
    };

    //==============================================================================

    bool enabledCombs[numCombs];
    bool enabledAllPasses[numAllPasses];  // Keep for backwards compatibility
    float allPassGains[numAllPasses];     // New gain control (0.0 to 1.0)

    Parameters parameters;
    float gain;

    CombFilter comb[numChannels][numCombs];
    AllPassFilter allPass[numChannels][numAllPasses];

    LinearSmoothedValue damping, feedback, dryGain, wetGain1, wetGain2;
    TempoSyncedRandomizer randomizer;
    TempoSyncedCrossfadeManager crossfadeManager;

public:
    TempoSyncedRandomizer& getRandomizer() { return randomizer; }
    const TempoSyncedRandomizer& getRandomizer() const { return randomizer; }
    
    TempoSyncedCrossfadeManager& getCrossfadeManager() { return crossfadeManager; }
    const TempoSyncedCrossfadeManager& getCrossfadeManager() const { return crossfadeManager; }
    
    void updateAllFilters(bool newCombStates[], bool newAllPassStates[]) {
        int fadeSamples = crossfadeManager.calculateFadeSamples();
        
        for (int i = 0; i < numCombs; ++i) {
            if (enabledCombs[i] != newCombStates[i]) {
                for (int ch = 0; ch < numChannels; ++ch) {
                    comb[ch][i].startFade(newCombStates[i], fadeSamples);
                }
                enabledCombs[i] = newCombStates[i];
            }
        }
        
        for (int i = 0; i < numAllPasses; ++i) {
            if (enabledAllPasses[i] != newAllPassStates[i]) {
                for (int ch = 0; ch < numChannels; ++ch) {
                    allPass[ch][i].startFade(newAllPassStates[i], fadeSamples);
                }
                enabledAllPasses[i] = newAllPassStates[i];
            }
        }
    }
};
