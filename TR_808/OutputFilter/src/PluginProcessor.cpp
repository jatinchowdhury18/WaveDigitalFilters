#include "PluginProcessor.h"

OutputFilterAudioProcessor::OutputFilterAudioProcessor()
{
    volumeParam = vts.getRawParameterValue ("volume");
    toneParam = vts.getRawParameterValue ("tone");
}

void OutputFilterAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    params.push_back (std::make_unique<VTSParam> ("volume", "Volume", String(), NormalisableRange { 0.0f, 1.0f }, 0.5f, &percentValToString, &stringToPercentVal));
    params.push_back (std::make_unique<VTSParam> ("tone", "Tone", String(), NormalisableRange { 0.0f, 1.0f }, 0.5f, &percentValToString, &stringToPercentVal));
}

void OutputFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        filter[ch].prepare (sampleRate * oversampling.getOversamplingFactor());
}

void OutputFilterAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        filter[ch].reset();
}

void OutputFilterAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = { osBlock.getChannelPointer (0), osBlock.getChannelPointer (1) };
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        filter[ch].setVolume (*volumeParam);
        filter[ch].setTone (*toneParam);

        auto* x = osBuffer.getWritePointer (ch);

        for (int n = 0; n < (int) osBuffer.getNumSamples(); ++n)
            x[n] = filter[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

AudioProcessorEditor* OutputFilterAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OutputFilterAudioProcessor();
}
