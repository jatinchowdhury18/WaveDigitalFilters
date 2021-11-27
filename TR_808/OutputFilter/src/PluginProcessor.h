#pragma once

#include "TR808OutputFilter.h"

class OutputFilterAudioProcessor : public chowdsp::PluginBase<OutputFilterAudioProcessor>
{
public:
    OutputFilterAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* volumeParam = nullptr;
    std::atomic<float>* toneParam = nullptr;

    OutputFilter filter[2];

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputFilterAudioProcessor)
};
