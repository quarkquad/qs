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
    qs::encode::Encoder encoder;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
