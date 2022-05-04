#pragma once

#include <chowdsp_plugin_utils/chowdsp_plugin_utils.h>

#include "SnareResonatorWDF.h"

class SnareResonatorAudioProcessor : public chowdsp::PluginBase<SnareResonatorAudioProcessor>
{
public:
    SnareResonatorAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* freqHzParam = nullptr;
    std::atomic<float>* resParam = nullptr;

    SnareResonatorWDF wdf[2];

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SnareResonatorAudioProcessor)
};
