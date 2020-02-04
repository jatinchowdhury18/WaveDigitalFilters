/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DiodeClipper.h"

//==============================================================================
/**
*/
class DiodeClipperAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DiodeClipperAudioProcessor();
    ~DiodeClipperAudioProcessor();

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

private:
    float* freqParam = nullptr;
    float* gainDBParam = nullptr;
    float* outDBParam = nullptr;
    
    float curGain = 1.0f;
    float oldGain = 1.0f;

    float curOutGain = 1.0f;
    float oldOutGain = 1.0f;

    AudioProcessorValueTreeState vts;
    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    DiodeClipper diodeClipper[2];

    dsp::Oversampling<float> oversampling;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodeClipperAudioProcessor)
};
