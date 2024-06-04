// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#include "MultiBandDecoder.h"
#include "BandDecoder.h"

namespace qs::decode {

MultiBandDecoder::MultiBandDecoder(std::vector<BandSpec> specs) {
    jassert(specs.size() > 0);

    for (auto &spec : specs) {
        auto decoder = std::make_unique<BandDecoder>(spec);
        bands.push_back(std::move(decoder));
    }
}

void MultiBandDecoder::process(AudioBuffer<float> &buffer) {
    const auto chanLF = (int)Channel::LF;
    const auto chanRF = (int)Channel::RF;
    const auto chanLB = (int)Channel::LB;
    const auto chanRB = (int)Channel::RB;

    hilbertL.process(buffer, chanLF, chanLB, chanLF);
    hilbertR.process(buffer, chanRF, chanRB, chanRF);

    // Filter and decode the input from the buffer into each band.
    for (auto &band : bands) {
        band->filterAndDecodeFrom(buffer);
    }

    // Sum the bands together to produce the final output.
    buffer.clear();
    for (auto &band : bands) {
        band->addTo(buffer);
    }
}

void MultiBandDecoder::prepare(float rate, int samplesPerBlock) {
    hilbertL.prepare(rate);
    hilbertR.prepare(rate);
    for (auto &band : bands) {
        band->prepare(rate, samplesPerBlock);
    }
}

void MultiBandDecoder::reset() {
    hilbertL.reset();
    hilbertR.reset();
    for (auto &band : bands) {
        band->reset();
    }
}

} // namespace qs::decode
