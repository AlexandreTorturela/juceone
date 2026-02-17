#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

//==============================================================================
class JuceOneAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit JuceOneAudioProcessorEditor (JuceOneAudioProcessor&);
    ~JuceOneAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JuceOneAudioProcessor& audioProcessor;

    juce::Slider gainSlider;
    juce::ToggleButton bypassButton;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> gainAttachment;
    std::unique_ptr<ButtonAttachment> bypassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceOneAudioProcessorEditor)
};
