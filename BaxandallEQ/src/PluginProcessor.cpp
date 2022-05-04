#include "PluginProcessor.h"

namespace
{
float skewParam (float val)
{
    val = std::pow (val, 3.333f);
    return juce::jlimit (0.01f, 0.99f, 1.0f - val);
}
} // namespace

BaxandallEqAudioProcessor::BaxandallEqAudioProcessor()
{
    bassParam = vts.getRawParameterValue ("bass");
    trebleParam = vts.getRawParameterValue ("treble");
    wdfParam = vts.getRawParameterValue ("wdf_choice");
}

void BaxandallEqAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    params.push_back (std::make_unique<VTSParam> ("bass", "Bass", juce::String(), juce::NormalisableRange { 0.0f, 1.0f }, 0.5f, &percentValToString, &stringToPercentVal));
    params.push_back (std::make_unique<VTSParam> ("treble", "Treble", juce::String(), juce::NormalisableRange { 0.0f, 1.0f }, 0.5f, &percentValToString, &stringToPercentVal));
    params.push_back (std::make_unique<juce::AudioParameterChoice> ("wdf_choice", "WDF", juce::StringArray { "Unadapted R-Type", "Adapted R-Type" }, 0));
}

void BaxandallEqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    prevWDF = (int) *wdfParam;

    const auto osSampleRate = sampleRate * (double) oversampling.getOversamplingFactor();
    for (int ch = 0; ch < 2; ++ch)
    {
        wdfUnadapted[ch].prepare (osSampleRate);
        wdfAdapted[ch].prepare (osSampleRate);

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

void BaxandallEqAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    int wdfType = (int) *wdfParam;
    if (wdfType != prevWDF)
    {
        for (int ch = 0; ch < 2; ++ch)
        {
            wdfUnadapted[ch].reset();
            wdfAdapted[ch].reset();
        }

        prevWDF = wdfType;
    }

    auto procWDFChannel = [=] (auto& wdf, float* x, int numSamples, int ch)
    {
        if (bassSmooth[ch].isSmoothing() || trebleSmooth[ch].isSmoothing())
        {
            for (int n = 0; n < numSamples; ++n)
            {
                wdf[ch].setParams (bassSmooth[ch].getNextValue(), trebleSmooth[ch].getNextValue());
                x[n] = wdf[ch].processSample (x[n]);
            }
        }
        else
        {
            wdf[ch].setParams (bassSmooth[ch].getNextValue(), trebleSmooth[ch].getNextValue());

            for (int n = 0; n < numSamples; ++n)
                x[n] = wdf[ch].processSample (x[n]);
        }
    };

    juce::dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    const int numSamples = (int) osBlock.getNumSamples();
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        bassSmooth[ch].setTargetValue (skewParam (*bassParam));
        trebleSmooth[ch].setTargetValue (skewParam (*trebleParam));

        auto* x = osBlock.getChannelPointer ((size_t) ch);
        if (wdfType == 0)
            procWDFChannel (wdfUnadapted, x, numSamples, ch);
        else
            procWDFChannel (wdfAdapted, x, numSamples, ch);
    }

    oversampling.processSamplesDown (block);

    buffer.applyGain (juce::Decibels::decibelsToGain (21.0f));
}

juce::AudioProcessorEditor* BaxandallEqAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BaxandallEqAudioProcessor();
}
