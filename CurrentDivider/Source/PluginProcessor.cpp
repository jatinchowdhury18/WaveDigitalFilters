/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
CurrentDividerAudioProcessor::CurrentDividerAudioProcessor()
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
    R1Param = vts.getRawParameterValue ("r1");
    R2Param = vts.getRawParameterValue ("r2");
}

CurrentDividerAudioProcessor::~CurrentDividerAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout CurrentDividerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    NormalisableRange<float> rRange (100.0f, 1000000.0f);
    rRange.setSkewForCentre (10000.0f);

    params.push_back (std::make_unique<AudioParameterFloat> ("r1", "R1 Value", rRange, 10000.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("r2", "R2 Value", rRange, 10000.0f));

    return { params.begin(), params.end() };
}

//==============================================================================
const String CurrentDividerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CurrentDividerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CurrentDividerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CurrentDividerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CurrentDividerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CurrentDividerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CurrentDividerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CurrentDividerAudioProcessor::setCurrentProgram (int index)
{
}

const String CurrentDividerAudioProcessor::getProgramName (int index)
{
    return {};
}

void CurrentDividerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CurrentDividerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);
}

void CurrentDividerAudioProcessor::releaseResources()
{
    oversampling.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CurrentDividerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void CurrentDividerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = {osBlock.getChannelPointer (0), osBlock.getChannelPointer (1)};
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

//==============================================================================
bool CurrentDividerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CurrentDividerAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void CurrentDividerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void CurrentDividerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new CurrentDividerAudioProcessor();
}
