#pragma once

#include <chowdsp_plugin_utils/chowdsp_plugin_utils.h>

#include "DiodeClipper.h"

class DiodeClipperAudioProcessor : public chowdsp::PluginBase<DiodeClipperAudioProcessor>
{
public:
    DiodeClipperAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* freqParam = nullptr;
    std::atomic<float>* gainDBParam = nullptr;
    std::atomic<float>* outDBParam = nullptr;

    float curGain = 1.0f;
    float oldGain = 1.0f;

    float curOutGain = 1.0f;
    float oldOutGain = 1.0f;

    DiodeClipper diodeClipper[2];

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodeClipperAudioProcessor)
};
