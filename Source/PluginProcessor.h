/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/Filter.h"
#include "DSP/Saturation.h"
#include "DSP/DCBlock.h"
#include "DSP/BrightVolume.h"
#include "DSP/FMVTonestack.h"

//==============================================================================
/**
*/
class PanOFlexAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    PanOFlexAudioProcessor();
    ~PanOFlexAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void parameterChanged(const juce::String& parameterID, float newValue) override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getValueTreeState();

    static juce::String paramVolume;
    static juce::String paramBright;
    static juce::String paramBass;
    static juce::String paramTreble;
    static juce::String paramMaster;
    static juce::String paramReverb;

    juce::AudioProcessorValueTreeState apvts;

private:

    juce::AudioProcessorValueTreeState::ParameterLayout createParams();

    Filter miller1;
    Filter miller2;
    Filter miller3;
    OnePoleHighPass rcfilter1;
    OnePoleHighPass rcfilter2;
    OnePoleHighPass rcfilter3;
    Saturation tube1;
    Saturation tube2;
    Saturation tube3;
    BrightVolume volumeControl;
    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParams;
    FMVTonestack tonestack;

    juce::dsp::Oversampling<float> oversampling{ getTotalNumOutputChannels(), 1, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple, false, true};

    float mVolume = 0.1f;
    bool mBright;
    float mMaster = 0.1f;
    float mReverb = 0.0f;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanOFlexAudioProcessor)
};
