// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <juce_dsp/juce_dsp.h>
#include <qs/qs.h>

const float epsilon = 0.0001f;
const float sampleRate = 44100.0f;

const auto fftOrder = 12;
const auto fftSize = 1 << fftOrder;

static Catch::Approx approx(float value) { return Catch::Approx(value).margin(epsilon); }

static std::vector<float>::size_type vecst_f(int i) { return static_cast<std::vector<float>::size_type>(i); }

static void sine(float *out, float frequency, int samples) {
    float deltaTime = 1.0f / sampleRate;
    float time = 0.0f;
    for (auto i = 0; i < samples; ++i) {
        out[i] = sinf(2.0f * float(M_PI) * frequency * time);
        time += deltaTime;
    }
}

TEST_CASE("hilbert: prepare sample rates") {
    qs::dsp::HilbertTransform t;

    t.prepare(11025.0f);
    t.prepare(22050.0f);
    t.prepare(44100.0f);
    t.prepare(48000.0f);
    t.prepare(96000.0f);
    t.prepare(192000.0f);
}

TEST_CASE("constants: encoder channels") {
    REQUIRE((int)qs::encode::Channel::LF == 0);
    REQUIRE((int)qs::encode::Channel::RF == 1);
    REQUIRE((int)qs::encode::Channel::LB == 2);
    REQUIRE((int)qs::encode::Channel::RB == 3);
}

TEST_CASE("constants: decoder channels") {
    REQUIRE((int)qs::decode::Channel::LF == 0);
    REQUIRE((int)qs::decode::Channel::RF == 1);
    REQUIRE((int)qs::decode::Channel::LB == 2);
    REQUIRE((int)qs::decode::Channel::RB == 3);
}

TEST_CASE("constants: controller channels") {
    REQUIRE((int)qs::decode::CtrlChannel::A == 0);
    REQUIRE((int)qs::decode::CtrlChannel::B == 1);
}

TEST_CASE("envelope: timing") {
    const int n = 50;
    AudioBuffer<float> buffer = AudioBuffer<float>(4, n);
    auto env = qs::dsp::Envelope({1.0f, 1.0f}, 1);

    auto b = buffer.getWritePointer(0);
    FloatVectorOperations::fill(b, 1.0f, n);

    env.prepare(sampleRate);
    env.process(buffer);

    REQUIRE(b[n - 1] == approx(0.9973f));
}

TEST_CASE("envelope: resting state") {
    const int n = 44100;
    AudioBuffer<float> buffer = AudioBuffer<float>(1, n);

    auto ch = buffer.getWritePointer(0);
    sine(ch, 100.0f, n);

    auto env = qs::dsp::Envelope({10.0f, 10.0f}, 1);
    env.prepare(sampleRate);
    env.process(buffer);

    REQUIRE(ch[n / 2] == approx(0.65409f));
}

TEST_CASE("controller: a = b") {
    const int n = 2048;
    AudioBuffer<float> buffer = AudioBuffer<float>(2, n);

    buffer.clear();

    auto a = buffer.getWritePointer(0);
    auto b = buffer.getWritePointer(1);
    FloatVectorOperations::fill(a, Decibels::decibelsToGain(-3.0f), n);
    FloatVectorOperations::fill(b, Decibels::decibelsToGain(-3.0f), n);

    qs::decode::Controller ctrl({1.0f, 1.0f}, 12.0f);
    ctrl.prepare(sampleRate);
    ctrl.process(buffer);

    CHECK(a[n - 1] == approx(0.5f));
    CHECK(b[n - 1] == approx(0.5f));
}

TEST_CASE("controller: a > b") {
    const int n = 2048;
    AudioBuffer<float> buffer = AudioBuffer<float>(2, n);

    auto a = buffer.getWritePointer(0);
    auto b = buffer.getWritePointer(1);
    FloatVectorOperations::fill(a, Decibels::decibelsToGain(-3.0f), n);
    FloatVectorOperations::fill(b, Decibels::decibelsToGain(-6.0f), n);

    qs::decode::Controller ctrl({1.0f, 1.0f}, 12.0f);
    ctrl.prepare(sampleRate);
    ctrl.process(buffer);

    CHECK(a[n - 1] == approx(0.625f));
    CHECK(b[n - 1] == approx(0.375f));
}

TEST_CASE("controller: a < b") {
    const int n = 2048;
    AudioBuffer<float> buffer = AudioBuffer<float>(2, n);

    auto a = buffer.getWritePointer(0);
    auto b = buffer.getWritePointer(1);
    FloatVectorOperations::fill(a, Decibels::decibelsToGain(-6.0f), n);
    FloatVectorOperations::fill(b, Decibels::decibelsToGain(-3.0f), n);

    qs::decode::Controller ctrl({1.0f, 1.0f}, 12.0f);
    ctrl.prepare(sampleRate);
    ctrl.process(buffer);

    CHECK(a[n - 1] == approx(0.375f));
    CHECK(b[n - 1] == approx(0.625f));
}

TEST_CASE("controller: a < b (max)") {
    const int n = 2048;
    AudioBuffer<float> buffer = AudioBuffer<float>(2, n);

    auto a = buffer.getWritePointer(0);
    auto b = buffer.getWritePointer(1);
    FloatVectorOperations::fill(a, Decibels::decibelsToGain(-12.0f), n);
    FloatVectorOperations::fill(b, 1.0f, n);

    qs::decode::Controller ctrl({1.0f, 1.0f}, 12.0f);
    ctrl.prepare(sampleRate);
    ctrl.process(buffer);

    // Dealing with epsilon around zero is an issue so just round it.
    float roundedA = fabs((a[n - 1] * 1000.0f) / 1000.0f);
    CHECK(roundedA == approx(0.0f));
    CHECK(b[n - 1] == approx(1.0f));
}

TEST_CASE("controller: a > b (max)") {
    const int n = 2048;
    AudioBuffer<float> buffer = AudioBuffer<float>(2, n);

    auto a = buffer.getWritePointer(0);
    auto b = buffer.getWritePointer(1);
    FloatVectorOperations::fill(a, 1.0f, n);
    FloatVectorOperations::fill(b, Decibels::decibelsToGain(-12.0f), n);

    qs::decode::Controller ctrl({1.0f, 1.0f}, 12.0f);
    ctrl.prepare(sampleRate);
    ctrl.process(buffer);

    CHECK(a[n - 1] == approx(1.0f));
    CHECK(b[n - 1] == approx(0.0f));
}

TEST_CASE("encoder: smoke") {
    const int n = 2048;
    AudioBuffer<float> buffer = AudioBuffer<float>(4, n);

    auto a = buffer.getWritePointer(0);
    auto b = buffer.getWritePointer(1);
    auto c = buffer.getWritePointer(2);
    auto d = buffer.getWritePointer(3);
    FloatVectorOperations::fill(a, 1.0f, n);
    FloatVectorOperations::fill(b, Decibels::decibelsToGain(-10.0f), n);
    FloatVectorOperations::fill(c, 1.0f, n);
    FloatVectorOperations::fill(d, Decibels::decibelsToGain(-10.0f), n);

    qs::encode::Encoder encoder;
    encoder.prepare(sampleRate, n);
    encoder.process(buffer);

    auto l = buffer.getReadPointer(0);
    CHECK(l[n - 1] != approx(0.0f));

    auto r = buffer.getReadPointer(1);
    CHECK(r[n - 1] != approx(0.0f));
}

TEST_CASE("decoder: static smoke") {
    const int n = 2048;
    AudioBuffer<float> buffer = AudioBuffer<float>(4, n);

    auto a = buffer.getWritePointer(0);
    auto b = buffer.getWritePointer(1);
    FloatVectorOperations::fill(a, 1.0f, n);
    FloatVectorOperations::fill(b, Decibels::decibelsToGain(-10.0f), n);

    // Empty spec is a decoder without controller action (static).
    qs::decode::Decoder decoder(qs::decode::Decoder::Spec{});
    decoder.prepare(sampleRate, n);
    decoder.process(buffer);

    auto o = buffer.getReadPointer(0);
    CHECK(o[n - 1] != approx(0.0f));
}

TEST_CASE("decoder: dualband smoke") {
    const int n = 2048;
    AudioBuffer<float> buffer = AudioBuffer<float>(4, n);

    // ; script/coefficients/generate --stops 2000 --order 1
    // 2000.0 [[0.11633650601051981, 0.11633650601051981, 0.0, 1.0, -0.7673269879789604, 0.0]]
    // 2000.0 [[0.8836634939894802, -0.8836634939894802, 0.0, 1.0, -0.7673269879789604, 0.0]]

    qs::decode::BandSpec low = {
        100.0f,
        100.0f,
        12.0f,
        12.0f,
        Decibels::decibelsToGain(0.0f),
        qs::decode::FilterCoefficients(0.11633650601051981, 0.11633650601051981, 0.0, 1.0, -0.7673269879789604, 0.0),
    };

    qs::decode::BandSpec high = {
        10.0f,
        10.0f,
        12.0f,
        12.0f,
        Decibels::decibelsToGain(0.0f),
        qs::decode::FilterCoefficients(0.019259274202335752, 0.019259274202335752, 0.0, 1.0, -0.9614814515953285, 0.0),
    };

    auto a = buffer.getWritePointer(0);
    auto b = buffer.getWritePointer(1);
    FloatVectorOperations::fill(a, 1.0f, n);
    FloatVectorOperations::fill(b, Decibels::decibelsToGain(-10.0f), n);

    qs::decode::MultiBandDecoder decoder({
        std::vector<qs::decode::BandSpec>({
            low,
            high,
        }),
    });
    decoder.prepare(sampleRate, n);
    decoder.process(buffer);

    auto o = buffer.getReadPointer(0);
    CHECK(o[n - 1] != approx(0.0f));
}

static float normalize(float v, float max) { return juce::jmap(v, 0.0f, juce::jmax(max, 1e-5f), 0.0f, 1.0f); }

static juce::Range<float> forwardFFT(float *inOut) {
    juce::dsp::FFT fft(fftOrder);
    fft.performFrequencyOnlyForwardTransform(inOut);
    auto rng = juce::FloatVectorOperations::findMinAndMax(inOut, fftSize / 2);
    for (auto i = 0; i < fftSize / 2; ++i) {
        inOut[i] = normalize(inOut[i], rng.getEnd());
    }
    return rng;
}

static int maxBin(float *buffer) {
    auto maxBin = 0;
    for (auto i = 0; i < fftSize / 2; ++i) {
        if (buffer[i] > buffer[maxBin]) {
            maxBin = i;
        }
    }
    return maxBin;
}

TEST_CASE("encoder -> decoder: different frequencies in each channel") {
    auto buffer = AudioBuffer<float>(4, fftSize);
    buffer.clear();

    sine(buffer.getWritePointer(0), 300.0f, fftSize);
    sine(buffer.getWritePointer(1), 1000.0f, fftSize);
    sine(buffer.getWritePointer(2), 3000.0f, fftSize);
    sine(buffer.getWritePointer(3), 65.0f, fftSize);

    qs::encode::Encoder encoder;
    encoder.prepare(sampleRate, fftSize);
    encoder.process(buffer);

    auto spec = qs::decode::Decoder::Spec{
        {1.0f, 1.0f},
        12.0f,
        12.0f,
    };
    qs::decode::Decoder decoder(spec);
    decoder.prepare(sampleRate, fftSize);
    decoder.process(buffer);

    // Make a copy of the data buffer, but expand it to 2x FFT size to make room
    // for transformation operations. We'll need that extra space empty for the
    // forward FFT.
    AudioBuffer<float> fftBuffer(buffer);
    fftBuffer.setSize(4, fftSize * 2, true, true);

    // bin == 1 / ((sampleRate / (2^fftOrder) / frequency))
    std::vector<int> maxBins = {28, 93, 279, 6};
    for (auto i = 0; i < 4; ++i) {
        auto ch = fftBuffer.getWritePointer(i);
        forwardFFT(ch);

        INFO("channel " << i);
        CHECK(ch[maxBin(ch)] == 1.0f);
        CHECK(ch[maxBins[vecst_f(i)]] == 1.0f);
        CHECK(maxBins[vecst_f(i)] == maxBin(ch));
    }
}

TEST_CASE("hilbert transform: phase coherence") {
    using juce::dsp::Complex;

    auto hilbertTransform = [&](float frequency) {
        // Channel layout:
        // 0: input
        // 1: HT(input).real
        // 2: HT(input).imag
        // 3: HT(HT(input).real | HT(input).imag).real
        // 4: HT(HT(input).real | HT(input).imag).imag
        auto buffer = AudioBuffer<float>(5, fftSize);
        buffer.clear();
        sine(buffer.getWritePointer(0), frequency, fftSize);

        // Calculate analytical signal of the input.
        qs::dsp::HilbertTransform ht;
        ht.process(buffer, 1, 2, 0);
        juce::HeapBlock<Complex<float>> originalComplex(fftSize);
        for (auto i = 0; i < fftSize; ++i) {
            auto htReal = buffer.getReadPointer(1);
            auto htImag = buffer.getReadPointer(2);
            originalComplex.getData()[i] = Complex<float>(htReal[i], htImag[i]);
        }

        // Calculate analytical signal for the real component of the input's analytical signal.
        ht.reset();
        ht.process(buffer, 3, 4, 1);
        juce::HeapBlock<Complex<float>> realHTComplex(fftSize);
        for (auto i = 0; i < fftSize; ++i) {
            auto htReal = buffer.getReadPointer(3);
            auto htImag = buffer.getReadPointer(4);
            realHTComplex.getData()[i] = Complex<float>(htReal[i], htImag[i]);
        }

        // Calculate analytical signal for the imaginary component of the input's analytical signal.
        ht.reset();
        ht.process(buffer, 3, 4, 2);
        juce::HeapBlock<Complex<float>> imagHTComplex(fftSize);
        for (auto i = 0; i < fftSize; ++i) {
            auto htReal = buffer.getReadPointer(3);
            auto htImag = buffer.getReadPointer(4);
            imagHTComplex.getData()[i] = Complex<float>(htReal[i], htImag[i]);
        }

        // It takes a few samples for the Hilbert Transform to warm up so we aren't
        // really interested in samples before this point. We're also using the
        // structure to test itself so there might be some cascade in the error.
        // This number is arbitrary, but puts us to a reasonably stable region of
        // the buffer.
        size_t startIndex = 100;

        float min(180.0f), max(-180.0f);

        // Compare the arguments of our secondary analytical signals to arrive at
        // some phase difference in degrees. Collect the minimum and maximum phases
        // so we can compare later.
        for (auto i = startIndex; i < fftSize; ++i) {
            auto phase = std::arg(imagHTComplex.getData()[i]) - std::arg(realHTComplex.getData()[i]);
            phase = std::atan2(std::sinf(phase), std::cosf(phase)); // wrap
            auto phaseDegrees = phase * 180 / float(M_PI);

            min = jmin(min, phaseDegrees);
            max = jmax(max, phaseDegrees);
        }

        return std::abs(max - min);
    };

    CHECK(hilbertTransform(400.0f) < 1.0f);
    CHECK(hilbertTransform(1000.0f) < 1.0f);
    CHECK(hilbertTransform(2000.0f) < 1.0f);
    CHECK(hilbertTransform(5000.0f) < 1.0f);
    CHECK(hilbertTransform(10000.0f) < 1.0f);
    CHECK(hilbertTransform(15000.0f) < 2.0f);
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
