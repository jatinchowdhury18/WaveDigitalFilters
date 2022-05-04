#include "PluginProcessor.h"

PulseShaperAudioProcessor::PulseShaperAudioProcessor() = default;

void PulseShaperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        shaper[ch].prepare (sampleRate * oversampling.getOversamplingFactor());
}

void PulseShaperAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        shaper[ch].reset();
}

void PulseShaperAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = osBlock.getChannelPointer ((size_t) ch);

        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            x[n] = shaper[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

juce::AudioProcessorEditor* PulseShaperAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PulseShaperAudioProcessor();
}
