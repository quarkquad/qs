// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#include "Envelope.h"

namespace qs::dsp {

Envelope::Envelope(EnvelopeTiming timing, int numChannels)
    : attackMS(timing.attackMS), releaseMS(timing.releaseMS), numChannels_(numChannels) {
    hist.calloc(numChannels_);
}

void Envelope::prepare(float sampleRate) {
    const float samplesPerMS = sampleRate * 0.001f;
    attack = powf(0.01f, 1.0f / (jmax(attackMS, 1.0f) * samplesPerMS));
    release = powf(0.01f, 1.0f / (jmax(releaseMS, 1.0f) * samplesPerMS));
}

void Envelope::processChannel(AudioBuffer<float> &buffer, int channel) {
    auto samples = buffer.getWritePointer(channel);
    const auto numSamples = buffer.getNumSamples();

    for (auto i = 0; i < numSamples; ++i) {
        auto v = samples[i] * samples[i];
        hist[channel] = (v > hist[channel] ? attack : release) * (hist[channel] - v) + v;
        samples[i] = sqrtf(hist[channel]);
    }
}

void Envelope::process(AudioBuffer<float> &buffer) {
    const auto numChannels = jmin(numChannels_, buffer.getNumChannels());

    for (auto i = 0; i < numChannels; ++i) {
        processChannel(buffer, i);
    }
}

void Envelope::reset() { hist.clear(numChannels_); }

} // namespace qs::dsp
