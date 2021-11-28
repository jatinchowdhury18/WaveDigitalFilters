#pragma once

#include "BaxandallBass.h"
#include "BaxandallTreble.h"

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

    BaxandallBass bBass[2];
    BaxandallTreble bTreble[2];

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaxandallEqAudioProcessor)
};
