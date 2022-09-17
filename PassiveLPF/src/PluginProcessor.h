#pragma once

#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <juce_dsp/juce_dsp.h>

#include "PassiveLPF.h"

class PassiveLpfAudioProcessor : public chowdsp::PluginBase<PassiveLpfAudioProcessor>
{
public:
    PassiveLpfAudioProcessor();

    static void addParameters (Parameters& params);

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* fcParam = nullptr;

    PassiveLPF LPF[2];

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PassiveLpfAudioProcessor)
};
