#include "PluginProcessor.h"

DiodeClipperAudioProcessor::DiodeClipperAudioProcessor()
{
    freqParam = vts.getRawParameterValue ("fc");
    gainDBParam = vts.getRawParameterValue ("gain");
    outDBParam = vts.getRawParameterValue ("out");
}

void DiodeClipperAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    juce::NormalisableRange<float>
        fcRange (20.0f, 20000.0f);
    fcRange.setSkewForCentre (1000.0f);

    params.push_back (std::make_unique<VTSParam> ("fc", "Cutoff [Hz]", juce::String(), fcRange, 1000.0f, &freqValToString, &stringToFreqVal));
    params.push_back (std::make_unique<VTSParam> ("gain", "Gain [dB]", juce::String(), juce::NormalisableRange { 0.0f, 30.0f }, 0.0f, &gainValToString, &stringToGainVal));
    params.push_back (std::make_unique<VTSParam> ("out", "Out Gain [dB]", juce::String(), juce::NormalisableRange { -30.f, 30.0f }, 0.0f, &gainValToString, &stringToGainVal));
}

void DiodeClipperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        diodeClipper[ch].prepare (sampleRate);

    curGain = juce::Decibels::decibelsToGain (gainDBParam->load());
    oldGain = curGain;

    curOutGain = juce::Decibels::decibelsToGain (outDBParam->load());
    oldOutGain = curOutGain;
}

void DiodeClipperAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        diodeClipper[ch].reset();
}

void DiodeClipperAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    // Input gain stage
    curGain = juce::Decibels::decibelsToGain (gainDBParam->load());

    if (oldGain == curGain)
    {
        buffer.applyGain (curGain);
    }
    else
    {
        buffer.applyGainRamp (0, buffer.getNumSamples(), oldGain, curGain);
        oldGain = curGain;
    }

    juce::dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    for (size_t ch = 0; ch < osBlock.getNumChannels(); ++ch)
    {
        diodeClipper[ch].setCircuitParams (*freqParam);
        auto* x = osBlock.getChannelPointer (ch);

        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            x[n] = diodeClipper[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);

    // Output gain stage
    curOutGain = juce::Decibels::decibelsToGain (outDBParam->load());

    if (oldOutGain == curOutGain)
    {
        buffer.applyGain (curOutGain);
    }
    else
    {
        buffer.applyGainRamp (0, buffer.getNumSamples(), oldOutGain, curOutGain);
        oldOutGain = curOutGain;
    }
}

juce::AudioProcessorEditor* DiodeClipperAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DiodeClipperAudioProcessor();
}
