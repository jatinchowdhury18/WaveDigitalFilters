#include "PluginProcessor.h"

CurrentDividerAudioProcessor::CurrentDividerAudioProcessor()
{
    R1Param = vts.getRawParameterValue ("r1");
    R2Param = vts.getRawParameterValue ("r2");
}

void CurrentDividerAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    NormalisableRange<float> rRange (100.0f, 1000000.0f);
    rRange.setSkewForCentre (10000.0f);

    params.push_back (std::make_unique<VTSParam> ("r1", "R1 Value", String(), rRange, 10000.0f, &floatValToString, &stringToFloatVal));
    params.push_back (std::make_unique<VTSParam> ("r2", "R2 Value", String(), rRange, 10000.0f, &floatValToString, &stringToFloatVal));
}

void CurrentDividerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);
}

void CurrentDividerAudioProcessor::releaseResources()
{
    oversampling.reset();
}

void CurrentDividerAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = { osBlock.getChannelPointer (0), osBlock.getChannelPointer (1) };
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        divider[ch].setResistorValues (*R1Param, *R2Param);
        auto* x = osBuffer.getWritePointer (ch);

        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = divider[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

AudioProcessorEditor* CurrentDividerAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CurrentDividerAudioProcessor();
}
