// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#include "Controller.h"

namespace qs::decode {

Controller::Controller(dsp::EnvelopeTiming timing, float initWidthDB) : env(timing, 2), widthDB(initWidthDB) {}

void Controller::process(AudioBuffer<float> &buffer) {
    const auto numSamples = buffer.getNumSamples();
    auto a = buffer.getWritePointer((int)CtrlChannel::A);
    auto b = buffer.getWritePointer((int)CtrlChannel::B);

    if (widthDB <= 0.0f) {
        FloatVectorOperations::fill(a, 0.5f, numSamples);
        FloatVectorOperations::fill(b, 0.5f, numSamples);
        return;
    }

    // Calculate an envelope that follows the signal.
    env.process(buffer);

    // Calculate the power difference of signals A and B in decibels.
    dsp::log10Channels(buffer);
    FloatVectorOperations::subtract(a, b, numSamples);
    FloatVectorOperations::multiply(a, 20.0f, numSamples);

    // Restrict by the width and scale to range [0,1].
    FloatVectorOperations::clip(a, a, -widthDB, widthDB, numSamples);
    FloatVectorOperations::add(a, widthDB, numSamples);
    FloatVectorOperations::multiply(a, 0.5f / widthDB, numSamples);

    // Wrap the result of A around the zero point to produce B.
    FloatVectorOperations::copy(b, a, numSamples);
    FloatVectorOperations::add(b, -1.0f, numSamples);
    FloatVectorOperations::abs(b, b, numSamples);
}

void Controller::prepare(float rate) { env.prepare(rate); }

void Controller::reset() { env.reset(); }

} // namespace qs::decode
