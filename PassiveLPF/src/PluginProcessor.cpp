#include "PluginProcessor.h"

PassiveLpfAudioProcessor::PassiveLpfAudioProcessor()
{
    fcParam = vts.getRawParameterValue ("fc");
}

void PassiveLpfAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    juce::NormalisableRange<float> fcRange (20.0f, 20000.0f);
    fcRange.setSkewForCentre (1000.0f);

    params.push_back (std::make_unique<VTSParam> ("fc", "Cutoff [Hz]", juce::String(), fcRange, 1000.0f, &freqValToString, &stringToFreqVal));
}

void PassiveLpfAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        LPF[ch].prepare (sampleRate);
}

void PassiveLpfAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        LPF[ch].reset();
}

void PassiveLpfAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);

    auto&& osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < (int) osBlock.getNumChannels(); ++ch)
    {
        LPF[ch].setCutoffFrequency (*fcParam);
        auto* x = osBlock.getChannelPointer ((size_t) ch);

        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            x[n] = LPF[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

juce::AudioProcessorEditor* PassiveLpfAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PassiveLpfAudioProcessor();
}
