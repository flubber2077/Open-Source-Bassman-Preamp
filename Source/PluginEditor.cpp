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
    setSize (700, 250);

    setSliderParams(volumeSlider);
    setSliderParams(bassSlider);
    setSliderParams(midSlider);
    setSliderParams(trebleSlider);
    setSliderParams(outputSlider);
    setSliderParams(reverbSlider);

    addAndMakeVisible(brightButton);
    brightButton.setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::black);
    brightButton.setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::black);
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
    auto darkGrey = juce::Colour(100, 100, 100);
    auto grey = juce::Colour(200, 200, 200);
    auto lightGrey = juce::Colour(235, 235, 235);
    auto black = juce::Colours::black;
    g.fillAll (primaryColor);

    auto area = getLocalBounds();
    auto banner = area.removeFromTop(30);

    g.setColour(pDarkColor);
    g.fillRect(banner);

    auto buttonArea = area.removeFromTop(30);
    g.setColour(primaryColor);
    g.fillRect(buttonArea);

    g.setColour (black);
    juce::Font mainComponentFont ("Haettenschweiler", 26.0f, juce::Font::plain);
    g.setFont(mainComponentFont);

    g.drawFittedText(TRANS("BASSMAN 5F6-A"), banner.removeFromLeft(200), juce::Justification::left, false);
    banner.removeFromRight(5);
    g.drawFittedText(TRANS("by Dylan Jordan"), banner.removeFromRight(200), juce::Justification::right, false);


    auto textArea = area.removeFromBottom(40);
    g.setColour(grey);
    g.fillRect(textArea);

    auto knobArea = area;
    g.setColour(lightGrey);
    g.fillRect(knobArea);

    g.setColour(black);

    const auto width = area.getWidth() / numControls;

    g.drawFittedText(TRANS("VOLUME"), textArea.removeFromLeft(width), juce::Justification::centred, false);
    g.drawFittedText(TRANS("BASS"), textArea.removeFromLeft(width), juce::Justification::centred, false);
    g.drawFittedText(TRANS("MIDDLE"), textArea.removeFromLeft(width), juce::Justification::centred, false);
    g.drawFittedText(TRANS("TREBLE"), textArea.removeFromLeft(width), juce::Justification::centred, false);
    g.drawFittedText(TRANS("OUTPUT"), textArea.removeFromLeft(width), juce::Justification::centred, false);
    g.drawFittedText(TRANS("REVERB"), textArea.removeFromLeft(width), juce::Justification::centred, false);
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
    midSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
    trebleSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
    outputSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(padding));
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
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(245, 245, 250));
    slider.setColour(juce::Slider::trackColourId, juce::Colour(109, 76, 65));
    slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 50, 25);
    slider.setTextBoxIsEditable(true);
    addAndMakeVisible(slider);
}
