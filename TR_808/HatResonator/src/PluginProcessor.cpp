#include "PluginProcessor.h"

HatResonatorAudioProcessor::HatResonatorAudioProcessor()
{
    freqHzParam = vts.getRawParameterValue ("freq");
    resParam = vts.getRawParameterValue ("res");
}

void HatResonatorAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    createFreqParameter (params, "freq", "Freq", 1000.0f, 20000.0f, 5000.0f, 5000.0f);
    createPercentParameter (params, "res", "Res.", 0.5f);
}

void HatResonatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        wdf[ch].prepare (sampleRate * (double) oversampling.getOversamplingFactor());
}

void HatResonatorAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        wdf[ch].reset();
}

void HatResonatorAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = { osBlock.getChannelPointer (0), osBlock.getChannelPointer (1) };
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        wdf[ch].setParameters (*freqHzParam, 1.0f - *resParam);

        auto* x = osBuffer.getWritePointer (ch);
        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = wdf[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

AudioProcessorEditor* HatResonatorAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HatResonatorAudioProcessor();
}
