#pragma once

#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <juce_dsp/juce_dsp.h>

#include "HatResonatorWDF.h"

class HatResonatorAudioProcessor : public chowdsp::PluginBase<HatResonatorAudioProcessor>
{
public:
    HatResonatorAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* freqHzParam = nullptr;
    std::atomic<float>* resParam = nullptr;

    HatResonatorWDF wdf[2];

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HatResonatorAudioProcessor)
};
