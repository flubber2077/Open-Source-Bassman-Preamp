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
    setSize (550, 200);

    setSliderParams(volumeSlider);
    setSliderParams(bassSlider);
    setSliderParams(trebleSlider);
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

    auto area = getLocalBounds();
    area = area.withTop(area.getBottom() - 40);

    const auto width = area.getWidth() / numControls;

    g.drawFittedText(TRANS("Volume"), area.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Bass"), area.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Treble"), area.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Master"), area.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Reverb"), area.removeFromLeft(width), juce::Justification::centred, 1);
}

void PanOFlexAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromBottom(40);

    auto bannerArea = area.removeFromTop(30);
    auto buttonArea = area.removeFromTop(30);

    auto knobArea = area;

    const auto knobWidth = knobArea.getWidth() / numControls;

    int padding = 0;

    volumeSlider.setBounds(knobArea.removeFromLeft (knobWidth).reduced (padding));
    bassSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
    trebleSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
    masterSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
    reverbSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));

    auto buttonWidth = buttonArea.getWidth() / numControls;

    auto brightArea = (buttonArea.removeFromLeft(buttonWidth));
    brightButton.setBounds(brightArea.reduced(25, 0));
}

void PanOFlexAudioProcessorEditor::setSliderParams(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 25);
    addAndMakeVisible(slider);
}
