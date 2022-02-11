/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PanOFlexAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PanOFlexAudioProcessorEditor (PanOFlexAudioProcessor&);
    ~PanOFlexAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void setSliderParams(juce::Slider& slider);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PanOFlexAudioProcessor& audioProcessor;

    int numControls = 5;

    juce::Slider volumeSlider;
    juce::Slider bassSlider;
    juce::Slider trebleSlider;
    juce::Slider masterSlider;
    juce::Slider reverbSlider;


    juce::ToggleButton brightButton;

    juce::AudioProcessorValueTreeState::SliderAttachment volumeAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramVolume, volumeSlider };
    juce::AudioProcessorValueTreeState::SliderAttachment masterAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramMaster, masterSlider };
    juce::AudioProcessorValueTreeState::SliderAttachment reverbAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramReverb, reverbSlider };
    juce::AudioProcessorValueTreeState::ButtonAttachment brightAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramBright, brightButton };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanOFlexAudioProcessorEditor)
};
