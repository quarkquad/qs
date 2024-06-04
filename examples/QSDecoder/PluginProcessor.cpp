#include "PluginProcessor.h"

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", AudioChannelSet::stereo(), true)
                         .withOutput("Output", AudioChannelSet::quadraphonic(), true)) {}

void PluginProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages) {
    juce::ignoreUnused(midiMessages);
    ScopedNoDenormals noDenormals;

    decoder.process(buffer);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    decoder.prepare(sampleRate, samplesPerBlock);
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
    return (layouts.getMainInputChannels() == 2 && layouts.getMainOutputChannels() == 4) ||
           (layouts.getMainInputChannelSet() == AudioChannelSet::stereo() &&
            layouts.getMainOutputChannelSet() == AudioChannelSet::quadraphonic());
}

void PluginProcessor::getStateInformation(MemoryBlock &destData) { juce::ignoreUnused(destData); }

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    juce::ignoreUnused(data);
    juce::ignoreUnused(sizeInBytes);
}

void PluginProcessor::releaseResources() {}
const String PluginProcessor::getName() const { return JucePlugin_Name; }
bool PluginProcessor::acceptsMidi() const { return false; }
bool PluginProcessor::producesMidi() const { return false; }
bool PluginProcessor::isMidiEffect() const { return false; }
double PluginProcessor::getTailLengthSeconds() const { return 0.0; }
int PluginProcessor::getNumPrograms() { return 1; }
int PluginProcessor::getCurrentProgram() { return 0; }
void PluginProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }
const String PluginProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
}
void PluginProcessor::changeProgramName(int index, const String &newName) {
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}
bool PluginProcessor::hasEditor() const { return true; }
AudioProcessorEditor *PluginProcessor::createEditor() { return new PluginEditor(*this); }
AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new PluginProcessor(); }

