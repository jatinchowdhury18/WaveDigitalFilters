#pragma once

#include "SnareResonatorWDF.h"

class SnareResonatorAudioProcessor : public chowdsp::PluginBase<SnareResonatorAudioProcessor>
{
public:
    SnareResonatorAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* RfbParam = nullptr;
    std::atomic<float>* RgParam = nullptr;
    std::atomic<float>* CParam = nullptr;

    SnareResonatorWDF wdf[2];

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SnareResonatorAudioProcessor)
};
