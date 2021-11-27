#pragma once

#include "PulseShaper.h"

class PulseShaperAudioProcessor : public chowdsp::PluginBase<PulseShaperAudioProcessor>
{
public:
    PulseShaperAudioProcessor();

    static void addParameters (Parameters& params) {}
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

private:
    PulseShaper shaper[2];

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulseShaperAudioProcessor)
};
