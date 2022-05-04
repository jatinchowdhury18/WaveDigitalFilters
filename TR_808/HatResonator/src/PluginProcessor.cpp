#include "PluginProcessor.h"

HatResonatorAudioProcessor::HatResonatorAudioProcessor()
{
    freqHzParam = vts.getRawParameterValue ("freq");
    resParam = vts.getRawParameterValue ("res");
}

void HatResonatorAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    createFreqParameter (params, "freq", "Freq", 1000.0f, 20000.0f, 5000.0f, 5000.0f);
    createPercentParameter (params, "res", "Res.", 0.5f);
}

void HatResonatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        wdf[ch].prepare (sampleRate * (double) oversampling.getOversamplingFactor());
}

void HatResonatorAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        wdf[ch].reset();
}

void HatResonatorAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        wdf[ch].setParameters (*freqHzParam, 1.0f - *resParam);

        auto* x = osBlock.getChannelPointer ((size_t) ch);
        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            x[n] = wdf[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

juce::AudioProcessorEditor* HatResonatorAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HatResonatorAudioProcessor();
}
