#pragma once

#include <chowdsp_plugin_utils/chowdsp_plugin_utils.h>
#include <juce_dsp/juce_dsp.h>

#include "TR808OutputFilter.h"

class OutputFilterAudioProcessor : public chowdsp::PluginBase<OutputFilterAudioProcessor>
{
public:
    OutputFilterAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* volumeParam = nullptr;
    std::atomic<float>* toneParam = nullptr;

    OutputFilter filter[2];

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputFilterAudioProcessor)
};
