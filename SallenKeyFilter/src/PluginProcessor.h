#pragma once

#include "SallenKeyLPFWDF.h"

class SallenKeyFilterAudioProcessor : public chowdsp::PluginBase<SallenKeyFilterAudioProcessor>
{
public:
    SallenKeyFilterAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* freqParam = nullptr;
    std::atomic<float>* qParam = nullptr;
    std::atomic<float>* filterTypeParam = nullptr;

    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> freqSmooth[2];
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> qSmooth[2];

    SallenKeyLPF lpfs[2];
    
    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SallenKeyFilterAudioProcessor)
};
