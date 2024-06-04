// Copyright 2024 dublab, et al.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../dsp/Envelope.h"
#include "../dsp/IIRFilter.h"
#include "../dsp/math.h"
#include <juce_audio_basics/juce_audio_basics.h>

using namespace juce;

namespace qs::decode {

enum class CtrlChannel { A, B };

// Controller produces two control coefficients for the Decoder by calculating
// the relative power difference between two input signals. The coefficients
// move in the range [0,1] and sum to 1. The two coefficients move in a seesaw
// manner; when one goes up the other goes down.
//
// The envelope timings dictate the shape of the envelope we derive from the
// input signals. The width restricts the allowed difference (in dB) between
// the derived signal envelopes. A width of 0 results in a static mid-point
// output for the coefficients. Together, timings and width become sensitivity
// controls for the Controller.
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
class Controller {
  public:
    Controller(dsp::EnvelopeTiming timing, float widthDB);

    // process calculates the control coefficients from a stereo signal.
    void process(AudioBuffer<float> &buffer);

    // prepare sets the sample rate.
    void prepare(float rate);

    // reset clears all internal state.
    void reset();

  private:
    dsp::Envelope env;

    float widthDB = 0.0f;
};

} // namespace qs::decode
