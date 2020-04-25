#pragma once

#include "PluginProcessor.h"
#include "PropertiesComponent.h"

class WdfprototyperAudioProcessorEditor : public AudioProcessorEditor,
                                          private ChangeListener
{
public:
    WdfprototyperAudioProcessorEditor (WdfprototyperAudioProcessor&);
    ~WdfprototyperAudioProcessorEditor();

    void refresh (Node* node, int center = 0);
    void paintConnectionTree (Node* node, Graphics& g);

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster* source) override;

private:
    WdfprototyperAudioProcessor& processor;

    std::unique_ptr<PropertiesComponent> propsComp;

    TooltipWindow tooltip { this, 500 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WdfprototyperAudioProcessorEditor)
};
