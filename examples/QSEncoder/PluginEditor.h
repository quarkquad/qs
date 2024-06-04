#pragma once

#include "PluginProcessor.h"

class PluginProcessor;

class PluginEditor : public juce::AudioProcessorEditor {
  public:
    PluginEditor(PluginProcessor &);

    void paint(Graphics &) override;

  private:
    PluginProcessor &pprocessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
