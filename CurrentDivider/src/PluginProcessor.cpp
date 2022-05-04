#include "PluginProcessor.h"

CurrentDividerAudioProcessor::CurrentDividerAudioProcessor()
{
    R1Param = vts.getRawParameterValue ("r1");
    R2Param = vts.getRawParameterValue ("r2");
}

void CurrentDividerAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    juce::NormalisableRange<float> rRange (100.0f, 1000000.0f);
    rRange.setSkewForCentre (10000.0f);

    params.push_back (std::make_unique<VTSParam> ("r1", "R1 Value", juce::String(), rRange, 10000.0f, &floatValToString, &stringToFloatVal));
    params.push_back (std::make_unique<VTSParam> ("r2", "R2 Value", juce::String(), rRange, 10000.0f, &floatValToString, &stringToFloatVal));
}

void CurrentDividerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate);
    oversampling.initProcessing (samplesPerBlock);
}

void CurrentDividerAudioProcessor::releaseResources()
{
    oversampling.reset();
}

void CurrentDividerAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    auto&& osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < (int) osBlock.getNumChannels(); ++ch)
    {
        divider[ch].setResistorValues (*R1Param, *R2Param);
        auto* x = osBlock.getChannelPointer ((size_t) ch);

        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            x[n] = divider[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

juce::AudioProcessorEditor* CurrentDividerAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CurrentDividerAudioProcessor();
}
