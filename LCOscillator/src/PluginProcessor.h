#pragma once

#include "LCOscillator.h"

class LcoscillatorAudioProcessor : public chowdsp::PluginBase<LcoscillatorAudioProcessor>
{
public:
    LcoscillatorAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* freqParam = nullptr;
    std::atomic<float>* switchParam = nullptr; // true -> switch closed

    LCOscillator lc[2];

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LcoscillatorAudioProcessor)
};
