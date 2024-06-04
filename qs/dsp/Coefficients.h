// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace qs::dsp {

const int COEFFICIENTS_RATE = 48000;

// Coefficients are second-order coefficients for IIRFilter.
struct Coefficients {
    double b0 = 1.0, b1 = 0.0, b2 = 0.0, a0 = 0.0, a1 = 0.0, a2 = 0.0;
    int sampleRate = COEFFICIENTS_RATE;

    Coefficients() = default;

    Coefficients(double _b0, double _b1, double _b2, double _a0, double _a1, double _a2)
        : b0{_b0}, b1{_b1}, b2{_b2}, a0{_a0}, a1{_a1}, a2{_a2} {}
};

} // namespace qs::dsp
