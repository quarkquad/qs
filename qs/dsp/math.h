// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cmath>
#include <juce_audio_basics/juce_audio_basics.h>

using namespace juce;

namespace qs::dsp {

const float root2 = sqrt(2.0f);

inline float safeLog10(float v) { return log10f(v + 1e-12f); }

inline void log10Channels(AudioBuffer<float> &buffer) {
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    for (auto channel = 0; channel < numChannels; ++channel) {
        auto samples = buffer.getWritePointer(channel);

        for (auto i = 0; i < numSamples; ++i) {
            samples[i] = safeLog10(samples[i]);
        }
    }
}

} // namespace qs::dsp
