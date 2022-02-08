/*
  ==============================================================================

    Controls.cpp
    Created: 5 Feb 2022 7:32:57pm
    Author:  Dylan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Controls.h"

//==============================================================================
ControlsComponent::ControlsComponent(juce::AudioProcessorValueTreeState& apvts)
{
    volumeAttachment = std::make_unique<SliderAttachment>(apvts, "VOLUME", volumeSlider);
    masterAttachment = std::make_unique<SliderAttachment>(apvts, "MASTER", masterSlider);
    reverbAttachment = std::make_unique<SliderAttachment>(apvts, "REVERB", reverbSlider);

    brightAttachment = std::make_unique< juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "BRIGHTSWITCH", brightButton);

    setSliderParams(volumeSlider);
    setSliderParams(masterSlider);
    setSliderParams(reverbSlider);

    addAndMakeVisible(brightButton);
}

ControlsComponent::~ControlsComponent()
{
}

void ControlsComponent::paint (juce::Graphics& g)
{

}

void ControlsComponent::resized()
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

void ControlsComponent::setSliderParams(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
}
