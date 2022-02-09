/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PanOFlexAudioProcessorEditor::PanOFlexAudioProcessorEditor (PanOFlexAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 250);

    setSliderParams(volumeSlider);
    setSliderParams(masterSlider);
    setSliderParams(reverbSlider);

    addAndMakeVisible(brightButton);
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
    g.setFont (22.0f);

    int numControls = 3;

    auto box = getLocalBounds().reduced(20);
    box = box.withTop(box.getBottom() - 40);

    const auto width = box.getWidth() / numControls;

    g.drawFittedText(TRANS("Volume"), box.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Master"), box.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Reverb"), box.removeFromLeft(width), juce::Justification::centred, 1);
}

void PanOFlexAudioProcessorEditor::resized()
{
    auto box = getLocalBounds().reduced(20);

    box.removeFromBottom(40);

    const auto width = box.getWidth() / 3;

    int padding = 10;

    volumeSlider.setBounds(box.removeFromLeft (width).reduced (padding));
    brightButton.setBounds(volumeSlider.getLocalBounds().reduced(padding));
    masterSlider.setBounds(box.removeFromLeft(width).reduced(padding));
    reverbSlider.setBounds(box.removeFromLeft(width).reduced(padding));
}

void PanOFlexAudioProcessorEditor::setSliderParams(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
}
