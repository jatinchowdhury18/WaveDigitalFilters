#pragma once

#include "Leafs/Leaf.h"
#include "Roots/RootNode.h"
#include "Source.h"

class WdfprototyperAudioProcessor : public chowdsp::PluginBase<WdfprototyperAudioProcessor>,
                                    private Node::Listener
{
public:
    WdfprototyperAudioProcessor();
    ~WdfprototyperAudioProcessor();

    static void addParameters (Parameters&) {}
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (AudioBuffer<float>&) override;

    AudioProcessorEditor* createEditor() override;

    void addNode (Node* node, Node* newNode) override;
    void replaceNode (Node* oldNode, Node* newNode) override;
    void changeProbe (Node* node) override;
    void changeInput (Node* newInput) override;
    void unprepare() override;

    void setProbe (Node* parent, Node* node);
    void setInput (Node* parent, Node* node);

    std::unique_ptr<RootNode> root;

private:
    Source* inputNode = nullptr;
    Leaf* probeNode = nullptr;

    std::atomic_bool isPrepared  { false };
    std::atomic_bool isRendering { false };

    bool isDeleting = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WdfprototyperAudioProcessor)
};