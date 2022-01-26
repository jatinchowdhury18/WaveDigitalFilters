#include "PluginProcessor.h"

SallenKeyFilterAudioProcessor::SallenKeyFilterAudioProcessor()
{
    freqParam = vts.getRawParameterValue ("freq");
    qParam = vts.getRawParameterValue ("q_val");
    filterTypeParam = vts.getRawParameterValue ("filter_choice");
}

void SallenKeyFilterAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    createFreqParameter (params, "freq", "Frequency", 20.0f, 20000.0f, 2000.0f, 2000.0f);
    emplace_param<VTSParam> (params, "q_val", "Q", String(), createNormalisableRange (0.2f, 10.0f, 0.7071f), 0.7071f, &floatValToString, &stringToFloatVal);
    emplace_param<AudioParameterChoice> (params, "filter_choice", "Type", StringArray { "LPF", "HPF", "BPF" }, 0);
}

void SallenKeyFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    const auto osSampleRate = sampleRate * (double) oversampling.getOversamplingFactor();
    for (int ch = 0; ch < 2; ++ch)
    {
        freqSmooth[ch].reset (osSampleRate, 0.02);
        qSmooth[ch].reset (osSampleRate, 0.02);

        lpfs[ch].prepare (osSampleRate);
    }
}

void SallenKeyFilterAudioProcessor::releaseResources()
{
}

void SallenKeyFilterAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    const int numSamples = (int) osBlock.getNumSamples();
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        freqSmooth[ch].setTargetValue (*freqParam);
        qSmooth[ch].setTargetValue (*qParam);

        auto* x = osBlock.getChannelPointer (ch);
        if (freqSmooth[ch].isSmoothing() || qSmooth[ch].isSmoothing())
        {
            for (int n = 0; n < numSamples; ++n)
            {
                lpfs[ch].setParams (freqSmooth[ch].getNextValue(), qSmooth[ch].getNextValue());
                x[n] = lpfs[ch].processSample (x[n]);
            }
        }
        else
        {
            lpfs[ch].setParams (freqSmooth[ch].getNextValue(), qSmooth[ch].getNextValue());

            for (int n = 0; n < numSamples; ++n)
                x[n] = lpfs[ch].processSample (x[n]);
        }
    }

    oversampling.processSamplesDown (block);
}

AudioProcessorEditor* SallenKeyFilterAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SallenKeyFilterAudioProcessor();
}
