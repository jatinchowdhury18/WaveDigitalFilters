#pragma once

#include "PassiveLPF.h"

class PassiveLpfAudioProcessor : public chowdsp::PluginBase<PassiveLpfAudioProcessor>
{
public:
    PassiveLpfAudioProcessor();

    static void addParameters (Parameters& params);

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* fcParam = nullptr;

    PassiveLPF LPF[2];

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PassiveLpfAudioProcessor)
};
