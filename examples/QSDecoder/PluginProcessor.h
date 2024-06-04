#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <qs/qs.h>

#include "PluginEditor.h"

using namespace juce;

class PluginProcessor : public AudioProcessor {
  public:
    PluginProcessor();

    // JUCE
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
    void processBlock(AudioBuffer<float> &, MidiBuffer &) override;
    AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;
    const String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String &newName) override;
    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

  private:
    // Coefficients generated with:
    //
    // $ script/coefficients/generate --stops 1000
    // 1000.0 [[0.061511768503621556, 0.061511768503621556, 0.0, 1.0, -0.8769764629927568, 0.0]]
    // 1000.0 [[0.9384882314963784, -0.9384882314963784, 0.0, 1.0, -0.8769764629927568, 0.0]]
    qs::decode::MultiBandDecoder decoder = {
        std::vector<qs::decode::BandSpec>({
            {
                20.0f,
                20.0f,
                10.0f,
                10.0f,
                Decibels::decibelsToGain(-6.0f),
                qs::decode::FilterCoefficients(0.061511768503621556, 0.061511768503621556, 0.0, 1.0,
                                               -0.8769764629927568, 0.0),
            },
            {
                20.0f,
                20.0f,
                10.0f,
                10.0f,
                Decibels::decibelsToGain(-6.0f),
                qs::decode::FilterCoefficients(0.9384882314963784, -0.9384882314963784, 0.0, 1.0, -0.8769764629927568,
                                               0.0),
            },
        }),
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
