// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#include "IIRFilter.h"

namespace qs::dsp {

IIRFilter::IIRFilter(Coefficients c) : coeffs{c}, coeffsQuant{c} {
    hist1.calloc(numChannels_);
    hist2.calloc(numChannels_);

    const double d = (coeffs.a2 - coeffs.a1 + 1.0);
    const double k = sqrt((coeffs.a1 + coeffs.a2 + 1.0) / d);

    q = k / ((2.0 - 2.0 * coeffs.a2) / d);
    arctanK = atan(k);
    vl = (coeffs.b0 + coeffs.b1 + coeffs.b2) / (coeffs.a1 + coeffs.a2 + 1.0);
    vb = (coeffs.b0 - coeffs.b2) / (1.0 - coeffs.a2);
    vh = (coeffs.b0 - coeffs.b1 + coeffs.b2) / (coeffs.a2 - coeffs.a1 + 1.0);
}

void IIRFilter::process(AudioBuffer<float> &buffer) {
    const auto numChannels = jmin(numChannels_, buffer.getNumChannels());

    const auto a1 = coeffsQuant.a1;
    const auto a2 = coeffsQuant.a2;
    const auto b0 = coeffsQuant.b0;
    const auto b1 = coeffsQuant.b1;
    const auto b2 = coeffsQuant.b2;

    for (auto channel = 0; channel < numChannels; ++channel) {
        auto samples = buffer.getWritePointer(channel);

        for (auto i = 0; i < buffer.getNumSamples(); ++i) {
            const auto in = samples[i];

            double f = in - a1 * hist1[channel] - a2 * hist2[channel];
            double out = b0 * f + b1 * hist1[channel] + b2 * hist2[channel];

            hist2[channel] = hist1[channel];
            hist1[channel] = f;

            samples[i] = float(out);
        }
    }
}

void IIRFilter::prepare(float sampleRate, int numChannels) {
    numChannels_ = numChannels;
    hist1.calloc(numChannels);
    hist2.calloc(numChannels);

    if (static_cast<int>(sampleRate) == coeffs.sampleRate) {
        coeffsQuant = coeffs;
        return;
    }

    auto targetRate = static_cast<double>(sampleRate);
    const auto coeffsSampleRate = static_cast<double>(coeffs.sampleRate);
    const double k = tan(arctanK * coeffsSampleRate / targetRate);
    const double k2 = k * k;
    const double kdq = k / q;

    const double a0 = k2 + kdq + 1.0;
    coeffsQuant.a1 = 2.0 * (k2 - 1.0) / a0;
    coeffsQuant.a2 = (1.0 - kdq + k2) / a0;

    const double l = vl * k2;
    const double b = vb * kdq;
    coeffsQuant.b0 = (l + b + vh) / a0;
    coeffsQuant.b1 = 2.0 * (l - vh) / a0;
    coeffsQuant.b2 = (l - b + vh) / a0;
}

void IIRFilter::reset() {
    hist1.clear(numChannels_);
    hist2.clear(numChannels_);
}

} // namespace qs::dsp
