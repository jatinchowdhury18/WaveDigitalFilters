#include "PluginProcessor.h"

BBDDelayAudioProcessor::BBDDelayAudioProcessor()
{
    delayTimeMsParam = vts.getRawParameterValue ("delay_time");
    nStagesParam = vts.getRawParameterValue ("n_stages");
    mixParam = vts.getRawParameterValue ("mix");
    inputFilterFreqParam = vts.getRawParameterValue ("input_freq");
    outputFilterFreqParam = vts.getRawParameterValue ("output_freq");

    feedbackSmooth.setParameterHandle (vts.getRawParameterValue ("feedback"));
}

void BBDDelayAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    emplace_param<VTSParam> (params, "delay_time", "Delay", juce::String(), createNormalisableRange (10.0f, 1000.0f, 250.0f), 200.0f, &timeMsValToString, &stringToTimeMsVal);
    emplace_param<juce::AudioParameterChoice> (params, "n_stages", "Stages", juce::StringArray { "256", "512", "1024", "2048" }, 1);
    createFreqParameter (params, "input_freq", "Input Filter Freq.", 1000.0f, 20000.0f, 5000.0f, 4000.0f);
    createFreqParameter (params, "output_freq", "Output Filter Freq.", 1000.0f, 20000.0f, 5000.0f, 5000.0f);
    createPercentParameter (params, "feedback", "Feedback", 0.0f);
    createPercentParameter (params, "mix", "Mix", 0.45f);
}

void BBDDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    const auto osSampleRate = sampleRate * (double) oversampling.getOversamplingFactor();
    const auto osSamplesPerBlock = samplesPerBlock * (int) oversampling.getOversamplingFactor();
    fs = (float) osSampleRate;

    clockSignal.prepare ({ osSampleRate, (juce::uint32) osSamplesPerBlock, 1 });

    for (auto& bbd : bbdDelays)
    {
        std::visit ([osSampleRate] (auto&& thisBBD)
                    { thisBBD->prepare (osSampleRate); },
                    bbd);
    }

    feedbackState = 0.0f;
    feedbackSmooth.prepare (osSampleRate, osSamplesPerBlock);
    feedbackSmooth.setRampLength (0.02);

    const auto&& spec = juce::dsp::ProcessSpec { sampleRate, (juce::uint32) samplesPerBlock, 1 };
    mixer.prepare (spec);
    mixer.setMixingRule (juce::dsp::DryWetMixingRule::linear);

    inputFilter.prepare (spec);
    outputFilter.prepare (spec);
}

void BBDDelayAudioProcessor::releaseResources()
{
    oversampling.reset();
}

void BBDDelayAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    const auto curBBDIndex = (int) *nStagesParam;
    if (curBBDIndex != prevBBDIndex)
    {
        std::visit ([] (auto&& bbd)
                    { bbd->reset(); },
                    bbdDelays[(size_t) prevBBDIndex]);
        prevBBDIndex = curBBDIndex;
    }

    // for now this effect is mono only
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();
    for (int ch = 1; ch < numChannels; ++ch)
        buffer.addFrom (0, 0, buffer, ch, 0, numSamples);
    buffer.applyGain (1.0f / (float) numChannels);

    auto&& block = juce::dsp::AudioBlock<float> { buffer }.getSingleChannelBlock (0);

    mixer.setWetMixProportion (*mixParam);
    mixer.pushDrySamples (block);

    inputFilter.setCutoffFrequency (*inputFilterFreqParam);
    inputFilter.processBlock (block);

    auto&& osBlock = oversampling.processSamplesUp (block);
    const auto osNumSamples = (int) osBlock.getNumSamples();
    feedbackSmooth.process (osNumSamples);

    std::visit (
        [fs = fs,
         delayTimeSeconds = delayTimeMsParam->load() * 0.001f,
         &clockSignal = clockSignal,
         data = osBlock.getChannelPointer (0),
         fbData = feedbackSmooth.getSmoothedBuffer(),
         &fbState = feedbackState,
         numSamples = osNumSamples] (auto&& bbd)
        {
            const auto bbdClockFreq = bbd->getClockRateHzForDelayTime (delayTimeSeconds);
            clockSignal.setFrequency (juce::jmin (bbdClockFreq, fs * 0.25f));

            for (int n = 0; n < numSamples; ++n)
            {
                const auto input = data[n] + 0.95f * fbData[n] * fbState;
                fbState = bbd->process (input, clockSignal.processSample());
                data[n] = fbState;
            }
        },
        bbdDelays[(size_t) curBBDIndex]);

    oversampling.processSamplesDown (block);

    outputFilter.setCutoffFrequency (*outputFilterFreqParam);
    outputFilter.processBlock (block);

    mixer.mixWetSamples (block);

    // split back out from mono
    for (int ch = 1; ch < numChannels; ++ch)
        buffer.copyFrom (ch, 0, buffer, 0, 0, numSamples);
}

juce::AudioProcessorEditor* BBDDelayAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BBDDelayAudioProcessor();
}
