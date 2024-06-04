// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../dsp/math.h"
#include "Controller.h"

namespace qs::decode {

enum class Channel { LF, RF, LB, RB };

// VariableMatrix is the matrix mathematics portion of the QS Vario Matrix
// decoder.
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
//
// QS Quadraphonic Synthesizer: What It Does and How It Does It
// Ryosuke Ito, Susumu Takahashi, Kouichi Hirano, Sansui Electronic Co., Ltd.
// 49th AES Convention, September 9-12, 1974
//
// The Principles and Performance of the Sansui QS Vario-matrix
// Susumu Takahashi, Kouichi Hirano, Sansui Electronic Co., Ltd.
// 44th AES Convention, February 20-22, 1973
//
// Matrix "Quad" Systems
// Wendy Carlos
// http://www.wendycarlos.com/surround/surround4.html
//
// Sansui QSD-1 Service Manual
// Sansui Electronic Co., Ltd.
class VariableMatrix {
  public:
    VariableMatrix(dsp::EnvelopeTiming timing, float lrWidthDB, float fbWidthDB);

    // process decodes a stereo signal and writes it back as a quadraphonic
    // signal.
    void process(AudioBuffer<float> &buffer);

    // prepare sets the sample rate and number of samples to be processed per
    // block of audio.
    void prepare(float rate, int samplesPerBlock);

    // reset clears all internal state.
    void reset();

  private:
    Controller ctrlLR, ctrlFB;
    AudioBuffer<float> bufferLR, bufferFB;
};

} // namespace qs::decode
