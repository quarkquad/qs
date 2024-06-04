// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#include "BandDecoder.h"

namespace qs::decode {

BandDecoder::BandDecoder(BandSpec spec)
    : filter(spec.coeffs.dspCoeffs()), matrix({spec.attackMS, spec.releaseMS}, spec.lrWidthDB, spec.fbWidthDB),
      gain(spec.gain) {

    buffer.setSize(4, 256);
}

void BandDecoder::filterAndDecodeFrom(AudioBuffer<float> &inBuffer) {
    buffer.makeCopyOf(inBuffer, true);
    filter.process(buffer);
    matrix.process(buffer);
}

void BandDecoder::addTo(AudioBuffer<float> &inBuffer) {
    const auto numChannels = buffer.getNumChannels();
    const auto numSamples = buffer.getNumSamples();

    for (int channel = 0; channel < numChannels; ++channel) {
        inBuffer.addFrom(channel, 0, buffer, channel, 0, numSamples, gain);
    }
}

void BandDecoder::prepare(float rate, int samplesPerBlock) {
    buffer.setSize(4, samplesPerBlock);
    buffer.clear();

    filter.prepare(rate, samplesPerBlock);
    matrix.prepare(rate, samplesPerBlock);
}

void BandDecoder::reset() {
    buffer.clear();
    filter.reset();
    matrix.reset();
}

} // namespace qs::decode
