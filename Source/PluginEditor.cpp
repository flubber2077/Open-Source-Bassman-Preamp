/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PanOFlexAudioProcessorEditor::PanOFlexAudioProcessorEditor (PanOFlexAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), controls(audioProcessor.apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(controls);
}

PanOFlexAudioProcessorEditor::~PanOFlexAudioProcessorEditor()
{
}

//==============================================================================
void PanOFlexAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void PanOFlexAudioProcessorEditor::resized()
{
    int width = getWidth();
    int height = getHeight();
    controls.setBounds(0, 30, width, height);
}
