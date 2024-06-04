// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "../dsp/HilbertTransform.h"
#include "../dsp/math.h"
#include "VariableMatrix.h"

using namespace juce;

namespace qs {

namespace decode {

// Decoder is an active, single-band QS decoder.
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
class Decoder {
  public:
    // Spec specifies parameters for creation of a Decoder.
    //
    // BandSpec is a superset of these options. See its documentation for more information.
    struct Spec {
        dsp::EnvelopeTiming timing;
        float lrWidthDB = 0.0f;
        float fbWidthDB = 0.0f;
    };

    Decoder(Spec spec);

    // process performs the decoding.
    void process(AudioBuffer<float> &buffer);

    // prepare sets the sample rate and expected block size.
    void prepare(float rate, int samplesPerBlock);

    // reset clears the internal state.
    void reset();

  private:
    dsp::HilbertTransform hilbertL, hilbertR;
    VariableMatrix matrix;
};

} // namespace decode

} // namespace qs
