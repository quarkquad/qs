// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cmath>
#include <juce_audio_basics/juce_audio_basics.h>

namespace qs::dsp {

struct EnvelopeTiming {
    float attackMS = 5.0f;
    float releaseMS = 5.0f;
};

// Envelope calculates the root mean squared value of a signal in a piece-wise
// manner.
//
// Attack and release timings are in milliseconds and limited to a minimum of 1 millisecond.
class Envelope {
  public:
    Envelope(EnvelopeTiming timing, int numChannels);

    // process processes a block of audio to calculate the Envelope.
    void process(juce::AudioBuffer<float> &buffer);

    // processChannel processes a single channel from a block of audio.
    void processChannel(juce::AudioBuffer<float> &buffer, int channel);

    // prepare sets the sample rate.
    void prepare(float sampleRate);

    // reset clears all internal state.
    void reset();

  private:
    juce::HeapBlock<float> hist;
    float attack = 0.0f, release = 0.0f;
    float attackMS = 0.0f, releaseMS = 0.0f;
    int numChannels_ = 0;
};

} // namespace qs::dsp
