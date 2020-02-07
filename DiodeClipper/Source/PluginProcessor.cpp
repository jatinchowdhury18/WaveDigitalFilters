/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
DiodeClipperAudioProcessor::DiodeClipperAudioProcessor()
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
    freqParam    = vts.getRawParameterValue ("fc");
    gainDBParam  = vts.getRawParameterValue ("gain");
    outDBParam   = vts.getRawParameterValue ("out");
    capLeakParam = vts.getRawParameterValue ("leak");
}

DiodeClipperAudioProcessor::~DiodeClipperAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout DiodeClipperAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    NormalisableRange<float> fcRange (20.0f, 20000.0f);
    fcRange.setSkewForCentre (1000.0f);

    params.push_back (std::make_unique<AudioParameterFloat> ("fc",   "Cutoff [Hz]", fcRange, 1000.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("gain", "Gain [dB]", 0.0f, 30.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("out",  "Out Gain [dB]", -30.f, 30.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("leak", "Cap Leakage", 0.0f, 1.0f, 0.0f));

    return { params.begin(), params.end() };
}

//==============================================================================
const String DiodeClipperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DiodeClipperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DiodeClipperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DiodeClipperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DiodeClipperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DiodeClipperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DiodeClipperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DiodeClipperAudioProcessor::setCurrentProgram (int index)
{
}

const String DiodeClipperAudioProcessor::getProgramName (int index)
{
    return {};
}

void DiodeClipperAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DiodeClipperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        diodeClipper[ch].reset (sampleRate, (double) *freqParam);

    curGain = Decibels::decibelsToGain (*gainDBParam);
    oldGain = curGain;

    curOutGain = Decibels::decibelsToGain (*outDBParam);
    oldOutGain = curOutGain;
}

void DiodeClipperAudioProcessor::releaseResources()
{
    oversampling.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DiodeClipperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DiodeClipperAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    // Input gain stage
    curGain = Decibels::decibelsToGain (*gainDBParam);

    if (oldGain == curGain)
    {
        buffer.applyGain (curGain);
    }
    else
    {
        buffer.applyGainRamp (0, buffer.getNumSamples(), oldGain, curGain);
        oldGain = curGain;
    }

    // WDF
    auto rParallel = 100.0e6 * pow (1.0 - *capLeakParam, 0.5) + 1e3;

    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = {osBlock.getChannelPointer (0), osBlock.getChannelPointer (1)};
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        diodeClipper[ch].setCircuitParams ((double) *freqParam, rParallel);
        auto* x = osBuffer.getWritePointer (ch);

        for (int n = 0; n < osBuffer.getNumSamples(); ++n)
            x[n] = diodeClipper[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);

    // Output gain stage
    curOutGain = Decibels::decibelsToGain (*outDBParam);

    if (oldOutGain == curOutGain)
    {
        buffer.applyGain (curOutGain);
    }
    else
    {
        buffer.applyGainRamp (0, buffer.getNumSamples(), oldOutGain, curOutGain);
        oldOutGain = curOutGain;
    }
}

//==============================================================================
bool DiodeClipperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DiodeClipperAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void DiodeClipperAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void DiodeClipperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new DiodeClipperAudioProcessor();
}
