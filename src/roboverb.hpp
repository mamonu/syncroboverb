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
    }
    
    void setEnabled(bool shouldBeEnabled) { enabled = shouldBeEnabled; }
    void setRate(RandomRate newRate) { rate = newRate; }
    void setAmount(float newAmount) { amount = juce::jlimit(0.0f, 1.0f, newAmount); }
    void setFilterType(FilterType newType) { filterType = newType; }
    
    bool isEnabled() const { return enabled; }
    RandomRate getRate() const { return rate; }
    float getAmount() const { return amount; }
    FilterType getFilterType() const { return filterType; }
    
    void processTempo(double bpm, double ppqPosition, class Roboverb& verb);
    
private:
    bool enabled;
    RandomRate rate;
    float amount;
    FilterType filterType;
    double lastPpqPosition;
    juce::Random rng;
    
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
    
    void randomizeSwitches(class Roboverb& verb);
};

class Roboverb {
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
        numParameters,
        numCombs = 8,
        numAllPasses = 4,
        numChannels = 2
    };

    Roboverb() {
        for (int i = 0; i < numCombs; ++i)
            enabledCombs[i] = false;
        enabledCombs[3] = true;
        enabledCombs[4] = true;
        enabledCombs[5] = true;

        for (int i = 0; i < numAllPasses; ++i)
            enabledAllPasses[i] = false;
        enabledAllPasses[0] = true;
        enabledAllPasses[1] = true;

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
              randomFilters (2.0f) {}

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
                    juce::exactlyEqual (randomFilters , o.randomFilters));
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

    void setParameters (const Parameters& newParams) {
        const float wetScaleFactor = 6.0f;
        const float dryScaleFactor = 2.0f;

        const float wet = newParams.wetLevel * wetScaleFactor;
        dryGain.setValue (newParams.dryLevel * dryScaleFactor);
        wetGain1.setValue (0.5f * wet * (1.0f + newParams.width));
        wetGain2.setValue (0.5f * wet * (1.0f - newParams.width));

        gain = isFrozen (newParams.freezeMode) ? 0.0f : 0.015f;
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
                if ( ! enabledAllPasses [j])
                    continue;
                outL = allPass[0][j].process (outL);
                outR = allPass[1][j].process (outR);
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
                // run the allpass filters in series
                if ( ! enabledAllPasses [j])
                    continue;
                output = allPass[0][j].process (output);
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
        CombFilter() noexcept : bufferSize (0), bufferIndex (0), last (0) {}

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

        float process (const float input, const float damp, const float feedbackLevel) noexcept {
            const float output = buffer[bufferIndex];
            last = (output * (1.0f - damp)) + (last * damp);
            JUCE_UNDENORMALISE (last);

            float temp = input + (last * feedbackLevel);
            JUCE_UNDENORMALISE (temp);
            buffer[bufferIndex] = temp;
            bufferIndex = (bufferIndex + 1) % bufferSize;
            return output;
        }

    private:
        std::unique_ptr<float[]> buffer;
        std::size_t bufferSize { 0 };
        std::size_t bufferIndex { 0 };
        float last;
    };

    //==============================================================================
    class AllPassFilter {
    public:
        AllPassFilter() noexcept : bufferSize (0), bufferIndex (0) {}

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

        float process (const float input) noexcept {
            const float bufferedValue = buffer[bufferIndex];
            float temp = input + (bufferedValue * 0.5f);
            // JUCE_UNDENORMALISE (temp);
            buffer[bufferIndex] = temp;
            bufferIndex = (bufferIndex + 1) % bufferSize;
            return bufferedValue - input;
        }

    private:
        std::unique_ptr<float[]> buffer;
        std::size_t bufferSize, bufferIndex;
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
    bool enabledAllPasses[numAllPasses];

    Parameters parameters;
    float gain;

    CombFilter comb[numChannels][numCombs];
    AllPassFilter allPass[numChannels][numAllPasses];

    LinearSmoothedValue damping, feedback, dryGain, wetGain1, wetGain2;
    TempoSyncedRandomizer randomizer;

public:
    TempoSyncedRandomizer& getRandomizer() { return randomizer; }
    const TempoSyncedRandomizer& getRandomizer() const { return randomizer; }
};
