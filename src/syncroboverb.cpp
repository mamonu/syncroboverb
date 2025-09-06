// Copyright (C) 2015-2025  Kushview, LLC <info@kushview.net>
// SPDX-License-Identifier: GPL3-or-later

#include "syncroboverb.hpp"

void TempoSyncedRandomizer::processTempo(double bpm, double ppqPosition, SyncRoboVerb& verb) {
    if (!enabled) return;
    
    double interval = getRateInQuarterNotes();
    
    // Check if we've moved past the next trigger point
    if (ppqPosition - lastPpqPosition >= interval) {
        randomizeSwitches(verb);
        lastPpqPosition = ppqPosition;
        switchesChanged = true;  // Mark that switches have been randomized
    }
}

void TempoSyncedRandomizer::randomizeSwitches(SyncRoboVerb& verb) {
    if (amount <= 0.0f) return;
    
    switch (filterType) {
        case CombsOnly:
            for (int i = 0; i < SyncRoboVerb::numCombs; ++i) {
                if (rng.nextFloat() < amount) {
                    bool currentState = verb.toggledCombFloat(i) > 0.5f;
                    verb.setCombToggle(i, !currentState);
                }
            }
            break;
            
        case AllPassOnly:
            for (int i = 0; i < SyncRoboVerb::numAllPasses; ++i) {
                if (rng.nextFloat() < amount) {
                    bool currentState = verb.toggledAllPassFloat(i) > 0.5f;
                    verb.setAllPassToggle(i, !currentState);
                }
            }
            break;
            
        case Both:
        default:
            // Randomize comb filters
            for (int i = 0; i < SyncRoboVerb::numCombs; ++i) {
                if (rng.nextFloat() < amount) {
                    bool currentState = verb.toggledCombFloat(i) > 0.5f;
                    verb.setCombToggle(i, !currentState);
                }
            }
            // Randomize all-pass filters
            for (int i = 0; i < SyncRoboVerb::numAllPasses; ++i) {
                if (rng.nextFloat() < amount) {
                    bool currentState = verb.toggledAllPassFloat(i) > 0.5f;
                    verb.setAllPassToggle(i, !currentState);
                }
            }
            break;
    }
}

void TempoSyncedRandomizer::getUpdatedSwitchStates(SyncRoboVerb& verb, BigInteger& combs, BigInteger& allpasses) {
    // Get current switch states from the reverb object
    combs.clear();
    allpasses.clear();
    
    for (int i = 0; i < SyncRoboVerb::numCombs; ++i) {
        combs.setBit(i, verb.toggledCombFloat(i) > 0.5f);
    }
    
    for (int i = 0; i < SyncRoboVerb::numAllPasses; ++i) {
        allpasses.setBit(i, verb.toggledAllPassFloat(i) > 0.5f);
    }
}
