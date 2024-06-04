// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#include "Encoder.h"

namespace qs::encode {

void Encoder::process(AudioBuffer<float> &buffer) {
    const auto numSamples = buffer.getNumSamples();

    const auto chanLF = (int)Channel::LF;
    const auto chanRF = (int)Channel::RF;
    const auto chanLB = (int)Channel::LB;
    const auto chanRB = (int)Channel::RB;

    auto leftFront = buffer.getWritePointer(chanLF);
    auto rightFront = buffer.getWritePointer(chanRF);
    auto leftBack = buffer.getWritePointer(chanLB);
    auto rightBack = buffer.getWritePointer(chanRB);

    float toss(0.0f), postL(0.0f), postR(0.0f), postLB(0.0f), postRB(0.0f);

    for (auto i = 0; i < numSamples; ++i) {
        auto lf = leftFront[i];
        auto rf = rightFront[i];
        auto lb = leftBack[i];
        auto rb = rightBack[i];

        // L and R are our reference angles
        hilbertLF.tick(postL, toss, lf + rf * coeff);
        hilbertRF.tick(postR, toss, rf + lf * coeff);

        // Phase shift LB and RB by -90 degrees
        hilbertLB.tick(toss, postLB, lb + rb * coeff);
        hilbertRB.tick(toss, postRB, rb + lb * coeff);

        leftFront[i] = postL - postLB;
        rightFront[i] = postR + postRB;
        leftBack[i] = 0.0f;
        rightBack[i] = 0.0f;
    }
}

void Encoder::prepare(float rate, int samplesPerBlock) {
    juce::ignoreUnused(samplesPerBlock);
    hilbertLF.prepare(rate);
    hilbertRF.prepare(rate);
    hilbertLB.prepare(rate);
    hilbertRB.prepare(rate);
}

void Encoder::reset() {
    hilbertLF.reset();
    hilbertRF.reset();
    hilbertLB.reset();
    hilbertRB.reset();
}

} // namespace qs::encode
