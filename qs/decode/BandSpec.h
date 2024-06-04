// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../dsp/Coefficients.h"

namespace qs::decode {

// FilterCoefficients are second-order coefficients to create a decoder
// frequency band filter.
struct FilterCoefficients {
    double b0 = 1.0, b1 = 0.0, b2 = 0.0, a0 = 0.0, a1 = 0.0, a2 = 0.0;

    FilterCoefficients() = default;

    FilterCoefficients(double _b0, double _b1, double _b2, double _a0, double _a1, double _a2)
        : b0{_b0}, b1{_b1}, b2{_b2}, a0{_a0}, a1{_a1}, a2{_a2} {}

    dsp::Coefficients dspCoeffs() { return {b0, b1, b2, a0, a1, a2}; }
};

// BandSpec specifies parameters for the creation of a frequency band within
// the MultiBandDecoder.
struct BandSpec {
    // Timings (milliseconds)
    //
    // attackMS and releaseMS are envelope timings for the band's controller logic. They control the
    // up-and-down movement of an envelopes that tracks the input signals. Shorter durations cause
    // the controllers to respond more quickly and longer durations cause them to respond more
    // slowly.
    float attackMS = 5.0f;
    float releaseMS = 5.0f;

    // Maximum Width (decibels)
    //
    // lrWidthDB and fbWidthDB specify the maximum dynamic range (in decibels) when deriving the
    // left-to-right and front-to-back controller coefficients. The lower the value, the less dynamic range available
    // and the more sensitive the controller coefficients will be. Conversely, higher values allow more dynamic range
    // and make the controller coefficients less sensitive. Widths of less-than or equal-to zero
    // will result in a static decoder band.
    float lrWidthDB = 0.0f;
    float fbWidthDB = 0.0f;

    // Post Gain
    //
    // gain is used when adding the Band's final signal back to the combined
    // output of the MultiBandDecoder.
    float gain = 1.0f;

    // Frequency Band Filter Coefficients
    //
    // coeffs are filter coefficients for the IIRFilter within the Band.
    // They should be generated for 48kHz, because the IIRFilter will
    // quantize them to other sample rates.
    FilterCoefficients coeffs;
};

} // namespace qs::decode
