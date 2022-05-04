#include "PluginProcessor.h"

SnareResonatorAudioProcessor::SnareResonatorAudioProcessor()
{
    freqHzParam = vts.getRawParameterValue ("freq");
    resParam = vts.getRawParameterValue ("res");
}

void SnareResonatorAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    createFreqParameter (params, "freq", "Freq", 80.0f, 2000.0f, 200.0f, 200.0f);
    createPercentParameter (params, "res", "Res.", 0.5f);
}

void SnareResonatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        wdf[ch].prepare (sampleRate * (double) oversampling.getOversamplingFactor());
}

void SnareResonatorAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        wdf[ch].reset();
}

void SnareResonatorAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        wdf[ch].setParameters (*freqHzParam, *resParam);

        auto* x = osBlock.getChannelPointer ((size_t) ch);
        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            x[n] = wdf[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

juce::AudioProcessorEditor* SnareResonatorAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SnareResonatorAudioProcessor();
}
