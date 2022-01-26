#include "PluginProcessor.h"

SnareResonatorAudioProcessor::SnareResonatorAudioProcessor()
{
    RfbParam = vts.getRawParameterValue ("r_fb");
    RgParam = vts.getRawParameterValue ("r_g");
    CParam = vts.getRawParameterValue ("c");
}

void SnareResonatorAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    emplace_param<VTSParam> (params, "r_fb", "R_fb [kOhms]", String(), NormalisableRange { 100.0f, 1000.0f }, 800.0f, &floatValToString, &stringToFloatVal);
    emplace_param<VTSParam> (params, "r_g", "R_g [Ohms]", String(), NormalisableRange { 500.0f, 5000.0f }, 600.0f, &floatValToString, &stringToFloatVal);
    emplace_param<VTSParam> (params, "c", "C [nF]", String(), NormalisableRange { 5.0f, 50.0f }, 20.0f, &floatValToString, &stringToFloatVal);
}

void SnareResonatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        wdf[ch].prepare (sampleRate * (double) oversampling.getOversamplingFactor());
}

void SnareResonatorAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        wdf[ch].reset();
}

void SnareResonatorAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = { osBlock.getChannelPointer (0), osBlock.getChannelPointer (1) };
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        wdf[ch].setParameters (*RfbParam * 1.0e3f, *RgParam, *CParam * 1.0e-9f);

        auto* x = osBuffer.getWritePointer (ch);
        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = wdf[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

AudioProcessorEditor* SnareResonatorAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SnareResonatorAudioProcessor();
}
