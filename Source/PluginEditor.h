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
    void setButtonParams(juce::Button& button);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PanOFlexAudioProcessor& audioProcessor;

    int numControls = 6;

    juce::Slider volumeSlider;
    juce::Slider bassSlider;
    juce::Slider midSlider;
    juce::Slider trebleSlider;
    juce::Slider outputSlider;
    juce::Slider reverbSlider;


    juce::ToggleButton brightButton;
    juce::ToggleButton cabinetButton;

    juce::AudioProcessorValueTreeState::SliderAttachment volumeAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramVolume, volumeSlider };
    juce::AudioProcessorValueTreeState::SliderAttachment bassAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramBass, bassSlider };
    juce::AudioProcessorValueTreeState::SliderAttachment midAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramMid, midSlider };
    juce::AudioProcessorValueTreeState::SliderAttachment trebleAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramTreble, trebleSlider };
    juce::AudioProcessorValueTreeState::SliderAttachment outputAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramOutput, outputSlider };
    juce::AudioProcessorValueTreeState::SliderAttachment reverbAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramReverb, reverbSlider };
    juce::AudioProcessorValueTreeState::ButtonAttachment brightAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramBright, brightButton };
    juce::AudioProcessorValueTreeState::ButtonAttachment cabinetAttachment{ audioProcessor.getValueTreeState(), PanOFlexAudioProcessor::paramCabinet, cabinetButton };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanOFlexAudioProcessorEditor)
};
