/*
  ==============================================================================

    Controls.h
    Created: 5 Feb 2022 7:32:57pm
    Author:  Dylan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ControlsComponent  : public juce::Component
{
public:
    ControlsComponent(juce::AudioProcessorValueTreeState& apvts);
    ~ControlsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void setSliderParams(juce::Slider& slider);

    juce::Slider volumeSlider;
    juce::Slider masterSlider;

    juce::ToggleButton brightButton;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> volumeAttachment;
    std::unique_ptr<SliderAttachment> masterAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> brightAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlsComponent)
};
