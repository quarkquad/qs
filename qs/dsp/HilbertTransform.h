// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

using namespace juce;

namespace qs::dsp {

// HilbertTransform calculates a Hilbert transform of an incoming signal.
// This is an IIR implementation using in-series 6th order all-pass filters.
// The imaginary component is -90 degrees out of phase with the real component.
//
// The Design of Wideband Analog 90° Phase Differencing Networks without Large Spread of Capacitor Values
// Bernie Hutchins
// Electronotes, Special Issue G, No. 168
// http://electronotes.netfirms.com/EN168-90degreePDN.PDF
//
// Calculation of the Poles of 90° Phase Difference Networks [after Weaver]
// Bernie Hutchins
// Musical Engineer's Handbook: Musical Engineering for Electronic Music. Ithaca, NY: Electronotes. 1975.
// http://electronotes.netfirms.com/MEHCh6aPart.PDF
//
// Design of RC Wide-Band 90-Degree Phase-Difference Network
// Donald K. Weaver
// Proceedings of the IRE, vol. 42, no. 4, 1954, pp. 671–676.
// http://ieeexplore.ieee.org/document/4051669/
class HilbertTransform {
  public:
    // Process calculates the analytical signal (Hilbert Transform) from some
    // input data. The output is a complex number. The real component is the
    // cosine component and the imag is the sine component. In more concrete
    // terms: The imaginary component is -90 degrees out of phase from the real
    // component.
    void process(AudioBuffer<float> &buf, int realChanNum, int imagChanNum, int srcChanNum);

    // tick processes a single sample.
    void tick(float &real, float &imaginary, float in);

    // prepare sets the sample rate.
    void prepare(float rate);

    // reset clears all internal state.
    void reset();

  private:
    void calculateCoefficients(float sampleRate);

    bool initialized = false;
    int poles = 0;
    HeapBlock<float> coeffs;
    HeapBlock<float> histX;
    HeapBlock<float> histY;
    float x1 = 0.0f;
    float x2 = 0.0f;
};

} // namespace qs::dsp
