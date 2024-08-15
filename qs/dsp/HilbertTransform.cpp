// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#include "HilbertTransform.h"

namespace qs::dsp {

void HilbertTransform::process(AudioBuffer<float> &buf, int realChanNum, int imagChanNum, int srcChanNum) {
    const auto numSamples = buf.getNumSamples();
    const auto srcChan = buf.getReadPointer(srcChanNum);
    auto realChan = buf.getWritePointer(realChanNum);
    auto imagChan = buf.getWritePointer(imagChanNum);

    for (auto i = 0; i < numSamples; ++i) {
        tick(realChan[i], imagChan[i], srcChan[i]);
    }
}

void HilbertTransform::tick(float &real, float &imaginary, float in) {
    real = 0.0f;
    imaginary = 0.0f;
    x1 = in;
    x2 = in;

    if (!initialized) {
        return;
    }

    const int bankPoles = poles / 2;

    // Calculate imaginary component (sine) with 6th order all-pass filter.
    for (int i = 0; i < bankPoles; ++i) {
        imaginary = coeffs[i] * (x1 - histY[i]) + histX[i];
        histX[i] = x1;
        histY[i] = imaginary;
        x1 = imaginary;
    }

    // Calculate real component (cosine) with 6th order all-pass filter.
    for (int i = bankPoles; i < poles; ++i) {
        real = coeffs[i] * (x2 - histY[i]) + histX[i];
        histX[i] = x2;
        histY[i] = real;
        x2 = real;
    }
}

void HilbertTransform::prepare(float sampleRate) {
    calculateCoefficients(sampleRate);
    initialized = true;
}

void HilbertTransform::calculateCoefficients(float sampleRate) {
    if (sampleRate == 0.0f) {
        return;
    }

    const float bandLow = 15.0f;
    const float bandHigh = sampleRate * 0.5f;
    const double bandRatio = bandLow / bandHigh;

    const double k = sqrt(1.0 - pow(bandRatio, 2.0));
    const double l = 0.5 * ((1.0 - sqrt(k)) / (1.0 + sqrt(k)));
    const double m = l + (2.0 * (pow(l, 5.0))) + (15.0 * (pow(l, 9.0)));
    const double q = exp((pow(M_PI, 2.0)) / log(m));

    const int n = int(ceil(log(0.2 * M_PI / 720.0) / log(q)));
    poles = n % 2 == 0 ? n : n + 1;
    coeffs.calloc(poles);
    histX.calloc(poles);
    histY.calloc(poles);

    auto calcPole = [&](float x) {
        const double y =
            atan(((pow(q, 2.0) - pow(q, 6.0)) * sin(4.0 * x)) / (1.0 + (pow(q, 2.0) + pow(q, 6.0)) * cos(4.0 * x)));
        return tan(x - y);
    };
    auto calcCoeff = [&](float p) {
        const float g = (bandLow * float(M_PI) * p) / sampleRate;
        return (g - 1.0f) / (g + 1.0f);
    };

    const float factor = M_PI / 4.0f / static_cast<float>(poles);
    const float scale = 1.0f / sqrtf(bandRatio);

    const int bankPoles = (poles / 2);
    for (auto i = 0; i < bankPoles; ++i) {
        const float r = 4.0f * (static_cast<float>(i) + 1);
        const float poleA = scale * calcPole(factor * (r - 3));
        const float poleB = scale * calcPole(factor * (r - 1));

        coeffs[i] = calcCoeff(poleA);
        coeffs[bankPoles + i] = calcCoeff(poleB);
    }

    reset();
}

void HilbertTransform::reset() {
    x1 = 0.0f;
    x2 = 0.0f;
    histX.clear(poles);
    histY.clear(poles);
}

} // namespace qs::dsp
