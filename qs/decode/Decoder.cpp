// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#include "Decoder.h"

namespace qs {

namespace decode {

Decoder::Decoder(Spec spec) : matrix(spec.timing, spec.lrWidthDB, spec.fbWidthDB) {}

void Decoder::process(AudioBuffer<float> &buffer) {
    hilbertL.process(buffer, (int)Channel::LF, (int)Channel::LB, (int)Channel::LF);
    hilbertR.process(buffer, (int)Channel::RF, (int)Channel::RB, (int)Channel::RF);

    matrix.process(buffer);
}

void Decoder::prepare(float rate, int samplesPerBlock) {
    hilbertL.prepare(rate);
    hilbertR.prepare(rate);
    matrix.prepare(rate, samplesPerBlock);
}

void Decoder::reset() {
    hilbertL.reset();
    hilbertR.reset();
    matrix.reset();
}

} // namespace decode

} // namespace qs
