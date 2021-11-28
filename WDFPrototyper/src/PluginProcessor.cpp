#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Roots/IdealVs.h"

WdfprototyperAudioProcessor::WdfprototyperAudioProcessor()
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

void WdfprototyperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    AudioProcessor::setRateAndBufferSizeDetails (sampleRate, samplesPerBlock);
    isPrepared = root->prepare (sampleRate);
}

void WdfprototyperAudioProcessor::releaseResources()
{
}

void WdfprototyperAudioProcessor::processAudioBlock (AudioBuffer<float>& buffer)
{
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
            root->getChild()->getWDF()->incident (root->getWDF()->reflected());

            // since we're probing a leaf, probe after WDF render
            x[n] = (float) probeNode->getWDF()->voltage();
        }
    }
    else
    {
        auto* x = buffer.getWritePointer (0);
        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            inputNode->input (x[n]);

            root->getWDF()->incident (root->getChild()->getWDF()->reflected());
            root->getChild()->getWDF()->incident (root->getWDF()->reflected());

            // since we're probing a leaf, probe after WDF render
            x[n] = (float) probeNode->getWDF()->voltage();
        }
    }

    if (buffer.getNumChannels() == 2)
        buffer.copyFrom (1, 0, buffer, 0, 0, buffer.getNumSamples());

    isRendering = false;
}

AudioProcessorEditor* WdfprototyperAudioProcessor::createEditor()
{
    return new WdfprototyperAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WdfprototyperAudioProcessor();
}
