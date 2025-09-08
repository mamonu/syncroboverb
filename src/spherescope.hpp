// Copyright (C) 2015-2025  Kushview, LLC <info@kushview.net>
// SPDX-License-Identifier: GPL3-or-later

#pragma once

#define SPHERE_DECAY_RATE (1.0f - 3E-6f)

#include "juce.hpp"
#include "res.hpp"

namespace syncroboverb {

class SphereScope : public Component {
public:
    SphereScope() {
        img = ImageCache::getFromMemory (res::sphere_scope_png,
                                         res::sphere_scope_pngSize);
        numSteps = img.getHeight() / img.getWidth();
        scale = (float) numSteps - 10.f;
        peakDecay = 1.0;
        peak = 0;
        peakHold = 0;
    }

    ~SphereScope() {}

    inline void setValue (const float val) {
        value = 10.f + Decibels::gainToDecibels (val, -70.0f);
        int level = getIECScale (value);
        if (peak < level) {
            peak = level;
            peakHold = 0;
            peakDecay = SPHERE_DECAY_RATE;

        } else if (++peakHold > 16) {
            peak = int (float (peak * peakDecay));
            if (peak < level) {
                peak = level;
            } else {
                peakDecay *= peakDecay;
            }
        }
    }

    inline void paint (Graphics& g) override {
        // Draw main sphere image
        g.drawImage (img, 0, 0, getWidth(), getHeight(), 0, img.getWidth() * peak, img.getWidth(), img.getWidth());
        
        // Add glow for high levels (above -20dB)
        if (value > -20.0f) {
            float glowStrength = (value + 20.0f) / 20.0f; // 0 to 1 for -20dB to 0dB
            glowStrength = juce::jlimit(0.0f, 1.0f, glowStrength);
            
            // Draw the same image again with glow effect - larger and semi-transparent
            g.setColour(Colour::fromFloatRGBA(1.0f, 1.0f, 0.8f, glowStrength * 0.5f));
            int glowExpansion = (int)(juce::jmax(getWidth(), getHeight()) * 0.1f); // 10% expansion for glow
            g.drawImage (img, -glowExpansion/2, -glowExpansion/2, 
                         getWidth() + glowExpansion, getHeight() + glowExpansion, 
                         0, img.getWidth() * peak, img.getWidth(), img.getWidth(),
                         false);
        }
        
        /* g.setColour (Colours::white);
        g.drawFittedText(String(getIECScale(value)), 0, 0, getWidth(), getHeight(),
                          Justification::centred, 1); */
    }

    inline int getIECScale (const float dB) const {
        float defaultScale = 1.0;

        if (dB < -70.0)
            defaultScale = 0.0;
        else if (dB < -60.0)
            defaultScale = (dB + 70.0) * 0.0025;
        else if (dB < -50.0)
            defaultScale = (dB + 60.0) * 0.005 + 0.025;
        else if (dB < -40.0)
            defaultScale = (dB + 50.0) * 0.0075 + 0.075;
        else if (dB < -30.0)
            defaultScale = (dB + 40.0) * 0.015 + 0.15;
        else if (dB < -20.0)
            defaultScale = (dB + 30.0) * 0.02 + 0.3;
        else // if (dB < 0.0)
            defaultScale = (dB + 20.0) * 0.025 + 0.5;

        int iecScale = defaultScale * scale;
        if (iecScale >= numSteps)
            iecScale = numSteps - 1;

        return iecScale;
    }

private:
    Image img;
    int numSteps;
    int index;
    float scale, value;
    int peak, peakHold;
    float peakDecay;
};

}
