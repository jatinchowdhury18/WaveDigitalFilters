/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Roots/RootNode.h"
#include "Leafs/Leaf.h"
#include "Source.h"

//==============================================================================
/**
*/
class WdfprototyperAudioProcessor  : public AudioProcessor,
                                     private Node::Listener
{
public:
    //==============================================================================
    WdfprototyperAudioProcessor();
    ~WdfprototyperAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    void addNode (Node* node, Node* newNode) override;
    void replaceNode (Node* oldNode, Node* newNode) override;
    void changeProbe (Node* node) override;
    void changeInput (Node* newInput) override;

    void setProbe (Node* parent, Node* node);
    void setInput (Node* parent, Node* node);

    std::unique_ptr<RootNode> root;

private:
    Source* inputNode = nullptr;
    Leaf* probeNode = nullptr;

    bool isPrepared = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WdfprototyperAudioProcessor)
};
