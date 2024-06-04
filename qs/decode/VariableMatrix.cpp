// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#include "VariableMatrix.h"

namespace qs::decode {

VariableMatrix::VariableMatrix(dsp::EnvelopeTiming timing, float lrWidthDB, float fbWidthDB)
    : ctrlLR(timing, lrWidthDB), ctrlFB(timing, fbWidthDB) {
    bufferLR.setSize(2, 256);
    bufferFB.setSize(2, 256);
    bufferLR.clear();
    bufferFB.clear();
}

void VariableMatrix::prepare(float rate, int samplesPerBlock) {
    // Control Buffers
    bufferLR.setSize(2, samplesPerBlock);
    bufferLR.clear();
    bufferFB.setSize(2, samplesPerBlock);
    bufferFB.clear();

    ctrlLR.prepare(rate);
    ctrlFB.prepare(rate);
}

void VariableMatrix::reset() {
    // Control Buffers
    bufferLR.clear();
    bufferFB.clear();
    ctrlLR.reset();
    ctrlFB.reset();
}

void VariableMatrix::process(AudioBuffer<float> &buffer) {
    const auto numSamples = buffer.getNumSamples();

    const auto chanLF = (int)Channel::LF;
    const auto chanRF = (int)Channel::RF;
    const auto chanLB = (int)Channel::LB;
    const auto chanRB = (int)Channel::RB;
    const auto ctrlChanA = (int)CtrlChannel::A;
    const auto ctrlChanB = (int)CtrlChannel::B;

    // Calculate left/right control coefficients.
    // Input A = L
    // Input B = R
    bufferLR.copyFrom(ctrlChanA, 0, buffer, chanLF, 0, numSamples);
    bufferLR.copyFrom(ctrlChanB, 0, buffer, chanRF, 0, numSamples);
    ctrlLR.process(bufferLR);

    // Calculate front/back control coefficients.
    // Input A = L + R
    // Input B = L - R
    bufferFB.copyFrom(ctrlChanA, 0, buffer, chanLF, 0, numSamples);
    bufferFB.addFrom(ctrlChanA, 0, buffer, chanRF, 0, numSamples);
    bufferFB.copyFrom(ctrlChanB, 0, buffer, chanLF, 0, numSamples);
    bufferFB.addFrom(ctrlChanB, 0, buffer, chanRF, 0, numSamples, -1.0);
    ctrlFB.process(bufferFB);

    // Scale control coefficients to sqrt(2).
    bufferLR.applyGain(dsp::root2);
    bufferFB.applyGain(dsp::root2);

    // Decoding
    auto leftFront = buffer.getWritePointer(chanLF);
    auto rightFront = buffer.getWritePointer(chanRF);
    auto leftBack = buffer.getWritePointer(chanLB);
    auto rightBack = buffer.getWritePointer(chanRB);

    // Control coefficient buffers. These will be populated by the controller
    // logic of ctrlLR and ctrlFB.
    auto ctrlLeft = bufferLR.getReadPointer(ctrlChanA);
    auto ctrlRight = bufferLR.getReadPointer(ctrlChanB);
    auto ctrlFront = bufferFB.getReadPointer(ctrlChanA);
    auto ctrlBack = bufferFB.getReadPointer(ctrlChanB);

    for (auto i = 0; i < numSamples; ++i) {
        auto lf = leftFront[i];
        auto rf = rightFront[i];
        auto lb = leftBack[i];
        auto rb = rightBack[i];
        auto cf = ctrlFront[i];
        auto cb = ctrlBack[i];
        auto cl = ctrlLeft[i];
        auto cr = ctrlRight[i];

        // QS Vario Matrix
        leftFront[i] = ((1 + cf) * (lf - rf)) + ((1 + cl) * dsp::root2 * rf);
        rightFront[i] = (-(1 + cf) * (lf - rf)) + ((1 + cr) * dsp::root2 * lf);
        leftBack[i] = ((1 + cb) * (lb + rb)) - ((1 + cl) * dsp::root2 * rb);
        rightBack[i] = ((1 + cb) * (lb + rb)) - ((1 + cr) * dsp::root2 * lb);
    }
}

} // namespace qs::decode
