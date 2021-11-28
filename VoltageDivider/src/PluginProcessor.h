#pragma once

#include "VoltageDivider.h"
#include <JuceHeader.h>

class VoltageDividerAudioProcessor : public chowdsp::PluginBase<VoltageDividerAudioProcessor>
{
public:
    VoltageDividerAudioProcessor();

    static void addParameters (Parameters& params);

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* R1Param = nullptr;
    std::atomic<float>* R2Param = nullptr;

    VoltageDivider divider[2];

    dsp::Oversampling<float> oversampling { 2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoltageDividerAudioProcessor)
};
