#pragma once

#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <juce_dsp/juce_dsp.h>

#include "SallenKeyLPFWDF.h"

class SallenKeyFilterAudioProcessor : public chowdsp::PluginBase<SallenKeyFilterAudioProcessor>
{
public:
    SallenKeyFilterAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* freqParam = nullptr;
    std::atomic<float>* qParam = nullptr;
    std::atomic<float>* filterTypeParam = nullptr;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> freqSmooth[2];
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> qSmooth[2];

    SallenKeyLPF lpfs[2];

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SallenKeyFilterAudioProcessor)
};
