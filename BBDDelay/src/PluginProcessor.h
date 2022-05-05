#pragma once

#include <chowdsp_plugin_utils/chowdsp_plugin_utils.h>
#include <juce_dsp/juce_dsp.h>

#include "BucketBrigadeWDF.h"

class BBDDelayAudioProcessor : public chowdsp::PluginBase<BBDDelayAudioProcessor>
{
public:
    BBDDelayAudioProcessor();

    static void addParameters (Parameters& params);

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* delayTimeMsParam = nullptr;
    std::atomic<float>* nStagesParam = nullptr;
    std::atomic<float>* mixParam = nullptr;
    std::atomic<float>* inputFilterFreqParam = nullptr;
    std::atomic<float>* outputFilterFreqParam = nullptr;
    chowdsp::SmoothedBufferValue<float> feedbackSmooth;

    BBD<256, float> bbd256;
    BBD<512, float> bbd512;
    BBD<1024, float> bbd1024;
    BBD<2048, float> bbd2048;

    using BBDVariant = std::variant<decltype (&bbd256),
                                    decltype (&bbd512),
                                    decltype (&bbd1024),
                                    decltype (&bbd2048)>;
    std::vector<BBDVariant> bbdDelays { &bbd256, &bbd512, &bbd1024, &bbd2048 };
    int prevBBDIndex = 0;
    float fs = 48000.0;
    float feedbackState = 0.0f;

    chowdsp::SineWave<float> clockSignal;

    juce::dsp::DryWetMixer<float> mixer;
    chowdsp::NthOrderFilter<float, 6> inputFilter, outputFilter;
    juce::dsp::Oversampling<float> oversampling { 1, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BBDDelayAudioProcessor)
};
