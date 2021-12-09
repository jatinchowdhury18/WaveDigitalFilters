#include "PluginProcessor.h"

LcoscillatorAudioProcessor::LcoscillatorAudioProcessor() {
    freqParam = vts.getRawParameterValue("fc");
    switchParam = vts.getRawParameterValue("switch");
}

void LcoscillatorAudioProcessor::addParameters(Parameters &params) {
    using namespace chowdsp::ParamUtils;

    NormalisableRange<float> fcRange(20.0f, 2000.0f);
    fcRange.setSkewForCentre(500.0f);

    params.push_back(std::make_unique<VTSParam>("fc", "Frequency [Hz]", String(), fcRange, 200.0f, &freqValToString,
                                                &stringToFreqVal));
    params.push_back(std::make_unique<AudioParameterBool>("switch", "Switch Closed", false));
}

void LcoscillatorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    oversampling.initProcessing(samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        lc[ch].prepare(sampleRate);
}

void LcoscillatorAudioProcessor::releaseResources() {
    oversampling.reset();

    for (int ch = 0; ch < 2; ++ch)
        lc[ch].reset();
}

void LcoscillatorAudioProcessor::processAudioBlock(AudioBuffer<float> &buffer) {
    dsp::AudioBlock<float> block(buffer);
    dsp::AudioBlock<float> osBlock(buffer);

    osBlock = oversampling.processSamplesUp(block);

    float *ptrArray[] = {osBlock.getChannelPointer(0), osBlock.getChannelPointer(1)};
    AudioBuffer<float> osBuffer(ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch) {
        lc[ch].setCircuitParams(*freqParam, (bool) *switchParam);
        auto *x = osBuffer.getWritePointer(ch);

        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = lc[ch].processSample(x[n]);
    }

    oversampling.processSamplesDown(block);

    buffer.applyGain(Decibels::decibelsToGain(-24.0f));
}

AudioProcessorEditor *LcoscillatorAudioProcessor::createEditor() {
    return new GenericAudioProcessorEditor(*this);
}

// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new LcoscillatorAudioProcessor();
}
