// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cmath>
#include <juce_audio_basics/juce_audio_basics.h>

namespace qs::dsp {

inline constexpr float root2 = 1.41421356f;

inline float safeLog10(float v) { return log10f(v + 1e-12f); }

inline void log10Channels(juce::AudioBuffer<float> &buffer) {
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
