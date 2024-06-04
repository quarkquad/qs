// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "../dsp/HilbertTransform.h"
#include "../dsp/math.h"

using namespace juce;

namespace qs::encode {

enum Channel { LF, RF, LB, RB };

const float coeff = dsp::root2 - 1.0f;

// Encoder is a static QS encoder. It encodes four distinct channels to a stereo
// signal in the QS Regular Matrix format (4-2).
//
// Four Channel Sound Reproduction System
// Ryosuke Ito; Toshiaki Ishida, Sansui Electronic Co., Ltd.
// US Patent 3757047A; September 4, 1973
//
// The Principles and Performance of the Sansui QS Vario-matrix
// Susumu Takahashi, Kouichi Hirano, Sansui Electronic Co., Ltd.
// 44th AES Convention, February 20-22, 1973
//
// QS Quadraphonic Synthesizer: What It Does and How It Does It
// Ryosuke Ito, Susumu Takahashi, Kouichi Hirano, Sansui Electronic Co., Ltd.
// 49th AES Convention, September 9-12, 1974
//
// Matrix "Quad" Systems
// Wendy Carlos
// http://www.wendycarlos.com/surround/surround4.html
class Encoder {
  public:
    // process encodes a four-channel input into a stereo output. The rear
    // channels are zeroed.
    void process(AudioBuffer<float> &buffer);

    // prepare sets the sample rate and number of samples to expect per block of
    // audio.
    void prepare(float rate, int samplesPerBlock);

    // reset clears all internal state.
    void reset();

  private:
    dsp::HilbertTransform hilbertLF, hilbertRF, hilbertLB, hilbertRB;
};

} // namespace qs::encode
