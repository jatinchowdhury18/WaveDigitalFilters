#pragma once

#include "PluginProcessor.h"
#include "PropertiesComponent.h"

class WdfprototyperAudioProcessorEditor : public juce::AudioProcessorEditor,
                                          private juce::ChangeListener
{
public:
    explicit WdfprototyperAudioProcessorEditor (WdfprototyperAudioProcessor&);
    ~WdfprototyperAudioProcessorEditor() override;

    void refresh (Node* node, int center = 0);
    void paintConnectionTree (Node* node, juce::Graphics& g);

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback (juce::ChangeBroadcaster* source) override;

private:
    WdfprototyperAudioProcessor& proc;

    std::unique_ptr<PropertiesComponent> propsComp;

    juce::TooltipWindow tooltip { this, 500 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WdfprototyperAudioProcessorEditor)
};
