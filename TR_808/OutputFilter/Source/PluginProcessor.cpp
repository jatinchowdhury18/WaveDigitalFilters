/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
OutputFilterAudioProcessor::OutputFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    vts (*this, nullptr, "Parameters", createParameterLayout()),
    oversampling (2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)
{
    volumeParam = vts.getRawParameterValue ("volume");
    toneParam = vts.getRawParameterValue ("tone");
}

OutputFilterAudioProcessor::~OutputFilterAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout OutputFilterAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back (std::make_unique<AudioParameterFloat> ("volume", "Volume", 0.0f, 1.0f, 0.5f));
    params.push_back (std::make_unique<AudioParameterFloat> ("tone",   "Tone",   0.0f, 1.0f, 0.5f));

    return { params.begin(), params.end() };
}

//==============================================================================
const String OutputFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OutputFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OutputFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OutputFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OutputFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OutputFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OutputFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OutputFilterAudioProcessor::setCurrentProgram (int index)
{
}

const String OutputFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void OutputFilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void OutputFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        filter[ch].reset (sampleRate * oversampling.getOversamplingFactor());
}

void OutputFilterAudioProcessor::releaseResources()
{
    oversampling.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OutputFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OutputFilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = {osBlock.getChannelPointer (0), osBlock.getChannelPointer (1)};
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        filter[ch].setVolume (*volumeParam);
        filter[ch].setTone (*toneParam);

        auto* x = osBuffer.getWritePointer (ch);

        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = filter[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

//==============================================================================
bool OutputFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* OutputFilterAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void OutputFilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void OutputFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (vts.state.getType()))
            vts.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OutputFilterAudioProcessor();
}
