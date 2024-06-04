// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../decode/BandSpec.h"
#include "VariableMatrix.h"

namespace qs::decode {

// BandDecoder decodes is an individual frequency band for the MultiBandDecoder.
// It assumes responsibility for filtering the input and delegating to a VariableMatrix
// for the matrix math.
//
// Inputs:
// 0: Left Total
// 1: Right Total
// 2: Left Total shifted -90 degrees
// 3: Right Total shifted -90 degrees
//
// Outputs:
// 0: Front Left
// 1: Front Right
// 2: Back Left
// 3: Back Right
class BandDecoder {
  public:
    BandDecoder(BandSpec spec);

    // filterAndDecodeFrom filters the input and decodes it to an internal buffer.
    void filterAndDecodeFrom(AudioBuffer<float> &buffer);

    // addTo adds the resulting filtered decode data to a buffer.
    void addTo(AudioBuffer<float> &buffer);

    // prepare sets the sample rate and expected block size.
    void prepare(float rate, int samplesPerBlock);

    // reset clears the internal state.
    void reset();

  private:
    AudioBuffer<float> buffer;
    dsp::IIRFilter filter;
    VariableMatrix matrix;

    float gain = 1.0f;
};

} // namespace qs::decode
