#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class WdfprototyperAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    WdfprototyperAudioProcessorEditor (WdfprototyperAudioProcessor&);
    ~WdfprototyperAudioProcessorEditor();

    void refresh (Node* node, int center = 0);
    void paintConnectionTree (Node* node, Graphics& g);

    void paint (Graphics&) override;
    void resized() override;

private:
    WdfprototyperAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WdfprototyperAudioProcessorEditor)
};
