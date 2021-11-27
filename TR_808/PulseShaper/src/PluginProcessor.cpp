#include "PluginProcessor.h"

PulseShaperAudioProcessor::PulseShaperAudioProcessor()
{
}

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

void PulseShaperAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = { osBlock.getChannelPointer (0), osBlock.getChannelPointer (1) };
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        auto* x = osBuffer.getWritePointer (ch);

        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = shaper[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

AudioProcessorEditor* PulseShaperAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PulseShaperAudioProcessor();
}
