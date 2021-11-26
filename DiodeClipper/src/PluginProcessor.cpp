#include "PluginProcessor.h"

DiodeClipperAudioProcessor::DiodeClipperAudioProcessor()
{
    freqParam    = vts.getRawParameterValue ("fc");
    gainDBParam  = vts.getRawParameterValue ("gain");
    outDBParam   = vts.getRawParameterValue ("out");
    capLeakParam = vts.getRawParameterValue ("leak");
}

void DiodeClipperAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    NormalisableRange<float>
        fcRange (20.0f, 20000.0f);
    fcRange.setSkewForCentre (1000.0f);

    params.push_back (std::make_unique<VTSParam> ("fc", "Cutoff [Hz]", String(), fcRange, 1000.0f, &freqValToString, &stringToFreqVal));
    params.push_back (std::make_unique<VTSParam> ("gain", "Gain [dB]", String(), NormalisableRange { 0.0f, 30.0f }, 0.0f, &gainValToString, &stringToGainVal));
    params.push_back (std::make_unique<VTSParam> ("out", "Out Gain [dB]", String(), NormalisableRange { -30.f, 30.0f }, 0.0f, &gainValToString, &stringToGainVal));
    //    params.push_back (std::make_unique<VTSParam> ("leak", "Cap Leakage", 0.0f, 1.0f, 0.0f));
}

void DiodeClipperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        diodeClipper[ch].prepare (sampleRate);

    curGain = Decibels::decibelsToGain (gainDBParam->load());
    oldGain = curGain;

    curOutGain = Decibels::decibelsToGain (outDBParam->load());
    oldOutGain = curOutGain;
}

void DiodeClipperAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        diodeClipper[ch].reset();
}

void DiodeClipperAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    // Input gain stage
    curGain = Decibels::decibelsToGain (gainDBParam->load());

    if (oldGain == curGain)
    {
        buffer.applyGain (curGain);
    }
    else
    {
        buffer.applyGainRamp (0, buffer.getNumSamples(), oldGain, curGain);
        oldGain = curGain;
    }

    dsp::AudioBlock<float> block (buffer);
    auto osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = {osBlock.getChannelPointer (0), osBlock.getChannelPointer (1)};
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        diodeClipper[ch].setCircuitParams (*freqParam);
        auto* x = osBuffer.getWritePointer (ch);

        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = diodeClipper[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);

    // Output gain stage
    curOutGain = Decibels::decibelsToGain (outDBParam->load());

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

AudioProcessorEditor* DiodeClipperAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DiodeClipperAudioProcessor();
}
