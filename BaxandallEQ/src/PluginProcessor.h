#pragma once

#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <juce_dsp/juce_dsp.h>

#include "BaxandallWDF.h"
#include "BaxandallWDFAdapt.h"

class BaxandallEqAudioProcessor : public chowdsp::PluginBase<BaxandallEqAudioProcessor>
{
public:
    BaxandallEqAudioProcessor();

    static void addParameters (Parameters& params);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* bassParam = nullptr;
    std::atomic<float>* trebleParam = nullptr;
    std::atomic<float>* wdfParam = nullptr;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> bassSmooth[2];
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> trebleSmooth[2];

    BaxandallWDF wdfUnadapted[2];
    BaxandallWDFAdapt wdfAdapted[2];

    int prevWDF = 0;

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaxandallEqAudioProcessor)
};
