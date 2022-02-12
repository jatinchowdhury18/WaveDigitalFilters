#pragma once

#include "HatResonatorWDF.h"

class HatResonatorAudioProcessor : public chowdsp::PluginBase<HatResonatorAudioProcessor>
{
public:
    HatResonatorAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* freqHzParam = nullptr;
    std::atomic<float>* resParam = nullptr;

    HatResonatorWDF wdf[2];

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HatResonatorAudioProcessor)
};
