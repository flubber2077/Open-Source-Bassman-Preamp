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

    setSliderParams(volumeSlider);
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
    auto numSliders = 1;
    auto sliderWidth = bounds.getWidth() / numSliders - padding;
    auto sliderHeight = bounds.getHeight() * 9 / 10;
    auto sliderStartX = padding;
    auto sliderStartY = 0;

    volumeSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
}

void ControlsComponent::setSliderParams(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    
    addAndMakeVisible(slider);
}
