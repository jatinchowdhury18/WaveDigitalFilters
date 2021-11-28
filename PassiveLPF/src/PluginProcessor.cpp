#include "PluginProcessor.h"

PassiveLpfAudioProcessor::PassiveLpfAudioProcessor()
{
    fcParam = vts.getRawParameterValue ("fc");
}

void PassiveLpfAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    NormalisableRange<float> fcRange (20.0f, 20000.0f);
    fcRange.setSkewForCentre (1000.0f);

    params.push_back (std::make_unique<VTSParam> ("fc", "Cutoff [Hz]", String(), fcRange, 1000.0f, &freqValToString, &stringToFreqVal));
}

void PassiveLpfAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);
    
    for (int ch = 0; ch < 2; ++ch)
        LPF[ch].prepare (sampleRate);
}

void PassiveLpfAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        LPF[ch].reset();
}

void PassiveLpfAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = { osBlock.getChannelPointer (0), osBlock.getChannelPointer (1) };
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        LPF[ch].setCutoffFrequency (*fcParam);
        auto* x = osBuffer.getWritePointer (ch);

        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = LPF[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

AudioProcessorEditor* PassiveLpfAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PassiveLpfAudioProcessor();
}
