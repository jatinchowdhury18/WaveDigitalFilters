#include "PluginProcessor.h"

BaxandallEqAudioProcessor::BaxandallEqAudioProcessor()
{
    bassParam = vts.getRawParameterValue ("bass");
    trebleParam = vts.getRawParameterValue ("treble");
}

void BaxandallEqAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    params.push_back (std::make_unique<VTSParam> ("bass", "Bass", String(), NormalisableRange { 0.0f, 1.0f }, 0.5f, &percentValToString, &stringToPercentVal));
    params.push_back (std::make_unique<VTSParam> ("treble", "Treble", String(), NormalisableRange { 0.0f, 1.0f }, 0.5f, &percentValToString, &stringToPercentVal));
}

void BaxandallEqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
    {
        bBass[ch].prepare (sampleRate * oversampling.getOversamplingFactor());
        bTreble[ch].prepare (sampleRate * oversampling.getOversamplingFactor());
    }
}

void BaxandallEqAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
    {
        bBass[ch].reset();
        bTreble[ch].reset();
    }
}

void BaxandallEqAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        bBass[ch].setBass (*bassParam);
        bTreble[ch].setTreble (*trebleParam);

        auto* x = osBlock.getChannelPointer (ch);

        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
        {
            x[n] = bBass[ch].processSample (x[n]) + bTreble[ch].processSample (x[n]);
        }
    }

    oversampling.processSamplesDown (block);
}

AudioProcessorEditor* BaxandallEqAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BaxandallEqAudioProcessor();
}
