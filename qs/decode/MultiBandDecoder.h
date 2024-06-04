// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "../decode/BandDecoder.h"
#include "../dsp/HilbertTransform.h"
#include "BandSpec.h"
#include <vector>

namespace qs::decode {

// MultiBandDecoder is an active, multi-band QS decoder.
//
// Inputs:
// 0: Left Total
// 1: Right Total
//
// Outputs:
// 0: Front Left
// 1: Front Right
// 2: Back Left
// 3: Back Right
//
// The inputs are split into multiple frequency bands so they can be controlled
// independently of one another. After decoding, the bands are summed together
// to produce the final output.
//
// QS Quadraphonic Synthesizer: What It Does and How It Does It
// Ryosuke Ito, Susumu Takahashi, Kouichi Hirano, Sansui Electronic Co., Ltd.
// 49th AES Convention, September 9-12, 1974
//
// Matrix "Quad" Systems
// Wendy Carlos
// http://www.wendycarlos.com/surround/surround4.html
//
// Sansui QSD-1 Service Manual
// Sansui Electronic Co., Ltd.
class MultiBandDecoder {
  public:
    MultiBandDecoder(std::vector<BandSpec> spec);

    // process performs the decoding.
    void process(juce::AudioBuffer<float> &buffer);

    // prepare sets the sample rate and expected block size.
    void prepare(float rate, int samplesPerBlock);

    // reset clears the internal state.
    void reset();

  private:
    dsp::HilbertTransform hilbertL, hilbertR;
    std::vector<std::unique_ptr<BandDecoder>> bands;
};

} // namespace qs::decode
