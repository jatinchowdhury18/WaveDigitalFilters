/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
LcoscillatorAudioProcessor::LcoscillatorAudioProcessor()
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
    vts (*this, nullptr, Identifier ("Parameters"), createParameterLayout()),
    oversampling (2, 1, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR)
{
    freqParam   = vts.getRawParameterValue ("fc");
    switchParam = vts.getRawParameterValue ("switch");
}

LcoscillatorAudioProcessor::~LcoscillatorAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout LcoscillatorAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    NormalisableRange<float> fcRange (20.0f, 2000.0f);
    fcRange.setSkewForCentre (500.0f);

    NormalisableRange<float> rRange (100.0f, 10000.0f);
    rRange.setSkewForCentre (1000.0f);

    params.push_back (std::make_unique<AudioParameterFloat> ("fc", "Frequency [Hz]", fcRange, 200.0f));
    params.push_back (std::make_unique<AudioParameterBool> ("switch", "Switch Closed", false));

    return { params.begin(), params.end() };
}

//==============================================================================
const String LcoscillatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LcoscillatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LcoscillatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LcoscillatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LcoscillatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LcoscillatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LcoscillatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LcoscillatorAudioProcessor::setCurrentProgram (int index)
{
}

const String LcoscillatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void LcoscillatorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LcoscillatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        lc[ch].reset (sampleRate, *freqParam, (bool) *switchParam);
}

void LcoscillatorAudioProcessor::releaseResources()
{
    oversampling.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LcoscillatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void LcoscillatorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = {osBlock.getChannelPointer (0), osBlock.getChannelPointer (1)};
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        lc[ch].setCircuitParams (*freqParam, (bool) *switchParam);
        auto* x = osBuffer.getWritePointer (ch);

        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = lc[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);

    buffer.applyGain (Decibels::decibelsToGain (-36.0f));
}

//==============================================================================
bool LcoscillatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LcoscillatorAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void LcoscillatorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void LcoscillatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new LcoscillatorAudioProcessor();
}
