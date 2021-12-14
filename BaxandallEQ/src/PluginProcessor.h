#pragma once

#include "BaxandallWDF.h"
#include "BaxandallWDFAdapt.h"

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
    std::atomic<float>* wdfParam = nullptr;

    SmoothedValue<float, ValueSmoothingTypes::Linear> bassSmooth[2];
    SmoothedValue<float, ValueSmoothingTypes::Linear> trebleSmooth[2];

    BaxandallWDF wdfUnadapted[2];
    BaxandallWDFAdapt wdfAdapted[2];

    int prevWDF = 0;

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaxandallEqAudioProcessor)
};
