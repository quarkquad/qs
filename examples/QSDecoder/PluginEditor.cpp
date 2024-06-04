#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor &p) : AudioProcessorEditor(&p), pprocessor(p) { setSize(250, 250); }

void PluginEditor::paint(Graphics &g) {
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 12.0f, Font::plain));

    const juce::String str = "QSDecoder Example: 2 inputs; 4 outputs\nInputs:\n 0: Total Left\n 1: Total "
                             "Right\nOutputs:\n 0: Front Left\n 1: Front Right\n "
                             "2: Rear Left\n 3: Rear Right\n";
    g.drawFittedText(str, 0, 0, getWidth(), getHeight(), Justification::left, 1);
}
