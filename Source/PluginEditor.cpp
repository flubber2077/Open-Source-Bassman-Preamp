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
    setSize (400, 300);

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
    g.setFont (15.0f);
}

void PanOFlexAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    auto padding = 10;
    auto numSliders = 3;
    auto sliderWidth = bounds.getWidth() / numSliders - padding;
    auto sliderHeight = bounds.getHeight() * 7 / 10;
    auto sliderStartX = 0;
    auto sliderStartY = 0;

    volumeSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    masterSlider.setBounds(volumeSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    reverbSlider.setBounds(masterSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    brightButton.setBounds(sliderStartX + (sliderWidth / 2) - padding, sliderStartY + sliderHeight + padding, 25, 25);
}

void PanOFlexAudioProcessorEditor::setSliderParams(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
}
