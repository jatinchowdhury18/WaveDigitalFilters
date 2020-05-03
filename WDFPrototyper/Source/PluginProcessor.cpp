/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Roots/IdealVs.h"

//==============================================================================
WdfprototyperAudioProcessor::WdfprototyperAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    root = std::make_unique<IdealVs>();
    root->addListener (this);
}

WdfprototyperAudioProcessor::~WdfprototyperAudioProcessor()
{
    isDeleting = true;
}

void WdfprototyperAudioProcessor::unprepare()
{
    while (isRendering)
        Thread::sleep (2);

    isPrepared = false;
}

void WdfprototyperAudioProcessor::addNode (Node* node, Node* newNode)
{
    newNode->addListener (this);

    if (auto editor = dynamic_cast<WdfprototyperAudioProcessorEditor*> (getActiveEditor()))
    {
        editor->refresh (node);
        editor->repaint();
    }

    isPrepared = root->prepare (getSampleRate());
}

void WdfprototyperAudioProcessor::replaceNode (Node* oldNode, Node* newNode)
{
    newNode->addListener (this);

    // update children of new node
    if (auto rootOldNode = dynamic_cast<RootNode*> (oldNode))
    {
        auto rootNewNode = dynamic_cast<RootNode*> (newNode);
        auto child = rootOldNode->getChildReleased();
        if (child != nullptr)
            rootNewNode->setChild (child);
    }
    else if (auto twoPortOldNode = dynamic_cast<TwoPort*> (oldNode))
    {
        auto twoPortNewNode = dynamic_cast<TwoPort*> (newNode);
        for (int idx = 0; idx < 2; ++idx)
        {
            auto child = twoPortOldNode->getChildReleased (idx);
            if (child != nullptr)
                twoPortNewNode->setChild (idx, child);
        }
    }

    // attach new node to parent
    if (auto rootNewNode = dynamic_cast<RootNode*> (newNode))
    {
        root.reset (rootNewNode);
    }
    else
    {
        auto parent = oldNode->getParent();

        if (auto rootParent = dynamic_cast<RootNode*> (parent))
        {
            rootParent->setChild (newNode);
        }
        else if (auto onePortParent = dynamic_cast<OnePort*> (parent))
        {
            onePortParent->setChild (newNode);
        }
        else if (auto twoPortParent = dynamic_cast<TwoPort*> (parent))
        {
            int idx = 0;
            for (; idx < 2; ++idx)
            {
                if (twoPortParent->getChild (idx) == oldNode)
                    break;
            }

            twoPortParent->setChild (idx, newNode);
        }
    }

    if (auto editor = dynamic_cast<WdfprototyperAudioProcessorEditor*> (getActiveEditor()))
    {
        editor->refresh (root.get());
        editor->repaint();
    }

    isPrepared = root->prepare (getSampleRate());
}

void WdfprototyperAudioProcessor::changeProbe (Node* node)
{
    probeNode = nullptr;

    if (isDeleting)
        return;

    setProbe (root.get(), node);

    if (node != nullptr)
        node->getCell()->repaint();
}

void WdfprototyperAudioProcessor::changeInput (Node* newInput)
{
    inputNode = nullptr;

    if (isDeleting)
        return;

    setInput (root.get(), newInput);
}

void WdfprototyperAudioProcessor::setProbe (Node* parent, Node* node)
{
    if (auto rootNode = dynamic_cast<RootNode*> (parent))
        setProbe (rootNode->getChild(), node);

    else if (auto onePortNode = dynamic_cast<OnePort*> (parent))
        setProbe (onePortNode->getChild(), node);

    else if (auto twoPortNode = dynamic_cast<TwoPort*> (parent))
    {
        setProbe (twoPortNode->getChild (0), node);
        setProbe (twoPortNode->getChild (1), node);
    }

    else if (auto leafNode = dynamic_cast<Leaf*> (parent))
    {
        if (parent != node)
            leafNode->setProbe (false);
        else
            probeNode = leafNode;
    }
}

void WdfprototyperAudioProcessor::setInput (Node* parent, Node* node)
{
    if (auto sourceNode = dynamic_cast<Source*> (parent))
    {
        if (parent != node)
            sourceNode->setInput (false);
        else
            inputNode = sourceNode;
    }

    if (auto rootNode = dynamic_cast<RootNode*> (parent))
        setInput (rootNode->getChild(), node);

    else if (auto onePortNode = dynamic_cast<OnePort*> (parent))
        setInput (onePortNode->getChild(), node);

    else if (auto twoPortNode = dynamic_cast<TwoPort*> (parent))
    {
        setInput (twoPortNode->getChild (0), node);
        setInput (twoPortNode->getChild (1), node);
    }
}

//==============================================================================
const String WdfprototyperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WdfprototyperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WdfprototyperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WdfprototyperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WdfprototyperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WdfprototyperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WdfprototyperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WdfprototyperAudioProcessor::setCurrentProgram (int index)
{
}

const String WdfprototyperAudioProcessor::getProgramName (int index)
{
    return {};
}

void WdfprototyperAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WdfprototyperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    AudioProcessor::setRateAndBufferSizeDetails (sampleRate, samplesPerBlock);
    isPrepared = root->prepare (sampleRate);
}

void WdfprototyperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WdfprototyperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void WdfprototyperAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    if (! isPrepared || probeNode == nullptr)
    {
        buffer.clear();
        return;
    }

    isRendering = true;

    if (inputNode == nullptr)
    {
        auto* x = buffer.getWritePointer (0);
        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            root->getWDF()->incident (root->getChild()->getWDF()->reflected());
            x[n] = (float) probeNode->getWDF()->voltage();
            root->getChild()->getWDF()->incident (root->getWDF()->reflected());
        }
    }
    else
    {
        auto* x = buffer.getWritePointer (0);
        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            inputNode->input (x[n]);

            root->getWDF()->incident (root->getChild()->getWDF()->reflected());
            x[n] = (float) probeNode->getWDF()->voltage();
            root->getChild()->getWDF()->incident (root->getWDF()->reflected());
        }
    }

    if (buffer.getNumChannels() == 2)
        buffer.copyFrom (1, 0, buffer, 0, 0, buffer.getNumSamples());

    isRendering = false;
}

//==============================================================================
bool WdfprototyperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WdfprototyperAudioProcessor::createEditor()
{
    return new WdfprototyperAudioProcessorEditor (*this);
}

//==============================================================================
void WdfprototyperAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WdfprototyperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WdfprototyperAudioProcessor();
}
