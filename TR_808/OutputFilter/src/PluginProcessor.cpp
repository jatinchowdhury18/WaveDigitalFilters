#include "PluginProcessor.h"

OutputFilterAudioProcessor::OutputFilterAudioProcessor()
{
    volumeParam = vts.getRawParameterValue ("volume");
    toneParam = vts.getRawParameterValue ("tone");
}

void OutputFilterAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    params.push_back (std::make_unique<VTSParam> ("volume", "Volume", juce::String(), juce::NormalisableRange { 0.0f, 1.0f }, 0.5f, &percentValToString, &stringToPercentVal));
    params.push_back (std::make_unique<VTSParam> ("tone", "Tone", juce::String(), juce::NormalisableRange { 0.0f, 1.0f }, 0.5f, &percentValToString, &stringToPercentVal));
}

void OutputFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        filter[ch].prepare (sampleRate * (double) oversampling.getOversamplingFactor());
}

void OutputFilterAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        filter[ch].reset();
}

void OutputFilterAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        filter[ch].setVolume (*volumeParam);
        filter[ch].setTone (*toneParam);

        auto* x = osBlock.getChannelPointer ((size_t) ch);
        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            x[n] = filter[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

juce::AudioProcessorEditor* OutputFilterAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OutputFilterAudioProcessor();
}
