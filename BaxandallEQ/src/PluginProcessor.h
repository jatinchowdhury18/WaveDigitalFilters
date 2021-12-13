#pragma once

#include "BaxandallWDF.h"

class BaxandallEqAudioProcessor : public chowdsp::PluginBase<BaxandallEqAudioProcessor>
{
public:
    BaxandallEqAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* bassParam = nullptr;
    std::atomic<float>* trebleParam = nullptr;

    SmoothedValue<float, ValueSmoothingTypes::Linear> bassSmooth[2];
    SmoothedValue<float, ValueSmoothingTypes::Linear> trebleSmooth[2];

    BaxandallWDF wdfCircuit[2];

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaxandallEqAudioProcessor)
};
