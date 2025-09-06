// Copyright (C) 2015-2025  Kushview, LLC <info@kushview.net>
// SPDX-License-Identifier: GPL3-or-later

#include "roboverb.hpp"

void TempoSyncedRandomizer::processTempo(double bpm, double ppqPosition, Roboverb& verb) {
    if (!enabled) return;
    
    double interval = getRateInQuarterNotes();
    
    // Check if we've moved past the next trigger point
    if (ppqPosition - lastPpqPosition >= interval) {
        randomizeSwitches(verb);
        lastPpqPosition = ppqPosition;
    }
}

void TempoSyncedRandomizer::randomizeSwitches(Roboverb& verb) {
    if (amount <= 0.0f) return;
    
    switch (filterType) {
        case CombsOnly:
            for (int i = 0; i < Roboverb::numCombs; ++i) {
                if (rng.nextFloat() < amount) {
                    bool currentState = verb.toggledCombFloat(i) > 0.5f;
                    verb.setCombToggle(i, !currentState);
                }
            }
            break;
            
        case AllPassOnly:
            for (int i = 0; i < Roboverb::numAllPasses; ++i) {
                if (rng.nextFloat() < amount) {
                    bool currentState = verb.toggledAllPassFloat(i) > 0.5f;
                    verb.setAllPassToggle(i, !currentState);
                }
            }
            break;
            
        case Both:
        default:
            // Randomize comb filters
            for (int i = 0; i < Roboverb::numCombs; ++i) {
                if (rng.nextFloat() < amount) {
                    bool currentState = verb.toggledCombFloat(i) > 0.5f;
                    verb.setCombToggle(i, !currentState);
                }
            }
            // Randomize all-pass filters
            for (int i = 0; i < Roboverb::numAllPasses; ++i) {
                if (rng.nextFloat() < amount) {
                    bool currentState = verb.toggledAllPassFloat(i) > 0.5f;
                    verb.setAllPassToggle(i, !currentState);
                }
            }
            break;
    }
}
