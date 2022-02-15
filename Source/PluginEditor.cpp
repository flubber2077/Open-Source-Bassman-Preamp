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
    setSize (700, 220);

    setSliderParams(volumeSlider);
    setSliderParams(bassSlider);
    setSliderParams(midSlider);
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

    auto primaryColor = juce::Colour(253, 216, 53);
    auto pDarkColor = juce::Colour(197, 166, 0);
    auto secondaryColor = juce::Colour(109, 76, 65);

    g.fillAll (primaryColor);

    auto area = getLocalBounds();
    auto banner = area.removeFromTop(18);

    g.setColour(pDarkColor);
    g.fillRect(banner);

    g.setColour (juce::Colours::black);
    g.setFont (22.0f);

    g.drawFittedText(TRANS("BASSMAN 5F6-A"), banner.removeFromLeft(200), juce::Justification::centred, 1);


    auto textArea = area.removeFromBottom(40);

    const auto width = area.getWidth() / numControls;

    g.drawFittedText(TRANS("Volume"), textArea.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Bass"), textArea.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Middle"), textArea.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Treble"), textArea.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Master"), textArea.removeFromLeft(width), juce::Justification::centred, 1);
    g.drawFittedText(TRANS("Reverb"), textArea.removeFromLeft(width), juce::Justification::centred, 1);
}

void PanOFlexAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromBottom(40);

    auto bannerArea = area.removeFromTop(18);
    auto buttonArea = area.removeFromTop(30);

    auto knobArea = area;

    const auto knobWidth = knobArea.getWidth() / numControls;

    int padding = 0;

    volumeSlider.setBounds(knobArea.removeFromLeft (knobWidth).reduced (padding));
    bassSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
    midSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
    trebleSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
    masterSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
    reverbSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));

    auto buttonWidth = buttonArea.getWidth() / numControls;

    auto brightArea = (buttonArea.removeFromLeft(buttonWidth));
    brightButton.setBounds(brightArea.reduced(25, 0));
}

void PanOFlexAudioProcessorEditor::setSliderParams(juce::Slider& slider)
{
    auto secondaryColor = juce::Colour(109, 76, 65);
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setColour(juce::Slider::thumbColourId, secondaryColor);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(255, 255, 107));
    slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 50, 25);
    slider.setTextBoxIsEditable(true);
    addAndMakeVisible(slider);
}
