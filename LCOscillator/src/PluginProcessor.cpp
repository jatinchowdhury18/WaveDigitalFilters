#include "PluginProcessor.h"

LcoscillatorAudioProcessor::LcoscillatorAudioProcessor()
{
    freqParam = vts.getRawParameterValue ("fc");
    switchParam = vts.getRawParameterValue ("switch");
}

void LcoscillatorAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    juce::NormalisableRange<float> fcRange (20.0f, 2000.0f);
    fcRange.setSkewForCentre (500.0f);

    params.push_back (std::make_unique<VTSParam> ("fc", "Frequency [Hz]", juce::String(), fcRange, 200.0f, &freqValToString, &stringToFreqVal));
    params.push_back (std::make_unique<juce::AudioParameterBool> ("switch", "Switch Closed", false));
}

void LcoscillatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        lc[ch].prepare (sampleRate);
}

void LcoscillatorAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        lc[ch].reset();
}

void LcoscillatorAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < (int) osBlock.getNumChannels(); ++ch)
    {
        lc[ch].setCircuitParams (*freqParam, (bool) *switchParam);
        auto* x = osBlock.getChannelPointer ((size_t) ch);

        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            x[n] = lc[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);

    buffer.applyGain (juce::Decibels::decibelsToGain (-24.0f));
}

juce::AudioProcessorEditor* LcoscillatorAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LcoscillatorAudioProcessor();
}
