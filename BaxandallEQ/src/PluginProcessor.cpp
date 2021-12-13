#include "PluginProcessor.h"

namespace
{
float skewParam (float val)
{
    val = std::pow (val, 3.333f);
    return jlimit (0.01f, 0.99f, 1.0f - val);
}
} // namespace

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

    const auto osSampleRate = sampleRate + oversampling.getOversamplingFactor();
    for (int ch = 0; ch < 2; ++ch)
    {
        wdfCircuit[ch].prepare (osSampleRate);

        bassSmooth[ch].reset (osSampleRate, 0.05);
        bassSmooth[ch].setCurrentAndTargetValue (skewParam (*bassParam));

        trebleSmooth[ch].reset (osSampleRate, 0.05);
        trebleSmooth[ch].setCurrentAndTargetValue (skewParam (*trebleParam));
    }
}

void BaxandallEqAudioProcessor::releaseResources()
{
    oversampling.reset();
}

void BaxandallEqAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    const int numSamples = (int) osBlock.getNumSamples();
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        bassSmooth[ch].setTargetValue (skewParam (*bassParam));
        trebleSmooth[ch].setTargetValue (skewParam (*trebleParam));

        auto* x = osBlock.getChannelPointer ((size_t) ch);
        if (bassSmooth[ch].isSmoothing() || trebleSmooth[ch].isSmoothing())
        {
            for (int n = 0; n < numSamples; ++n)
            {
                wdfCircuit[ch].setParams (bassSmooth[ch].getNextValue(), trebleSmooth[ch].getNextValue());
                x[n] = wdfCircuit[ch].processSample (x[n]);
            }
        }
        else
        {
            wdfCircuit[ch].setParams (bassSmooth[ch].getNextValue(), trebleSmooth[ch].getNextValue());

            for (int n = 0; n < numSamples; ++n)
                x[n] = wdfCircuit[ch].processSample (x[n]);
        }
    }

    oversampling.processSamplesDown (block);

    buffer.applyGain (Decibels::decibelsToGain (21.0f));
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
