#pragma once

#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <juce_dsp/juce_dsp.h>

#include "CurrentDivider.h"

class CurrentDividerAudioProcessor : public chowdsp::PluginBase<CurrentDividerAudioProcessor>
{
public:
    CurrentDividerAudioProcessor();

    static void addParameters (Parameters& params);

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* R1Param = nullptr;
    std::atomic<float>* R2Param = nullptr;

    CurrentDivider divider[2];

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CurrentDividerAudioProcessor)
};
