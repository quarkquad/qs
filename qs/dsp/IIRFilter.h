// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "Coefficients.h"

using namespace juce;

namespace qs::dsp {

// IIRFilter is a second-order Butterworth filter.
//
// Unless you know exactly the sample rate the filter will operate at, coefficients should be
// calculated at 48kHz. The filter will quantize the coefficients if a different sample rate is
// used.
//
// Implementation and Evaluation of Autonomous Multi-track Fader Control
// Mansbridge
//
// Parameter Quantization in Direct-Form Recursive Audio Filters
// Neunaber
class IIRFilter {
  public:
    IIRFilter(dsp::Coefficients coeffs48);

    // prepare sets the sample rate and number of channels to be processed by
    // the filter.
    void prepare(float sampleRate, int numChannels);

    // processBlock processes a buffer of audio. It only processes channel
    // indexes up until the number specified in prepare.
    void process(AudioBuffer<float> &buffer);

    // reset clears all internal state.
    void reset();

  private:
    int numChannels_ = 1;

    Coefficients coeffs;
    Coefficients coeffsQuant;

    double q, vh, vb, vl, arctanK;

    HeapBlock<double> hist1;
    HeapBlock<double> hist2;
};

} // namespace qs::dsp
