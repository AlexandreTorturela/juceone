#include "PluginEditor.h"

//==============================================================================
JuceOneAudioProcessorEditor::JuceOneAudioProcessorEditor (JuceOneAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    gainSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 20);
    gainSlider.setTextValueSuffix (" dB");
    addAndMakeVisible (gainSlider);

    bypassButton.setButtonText ("Bypass");
    addAndMakeVisible (bypassButton);

    gainAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, JuceOneAudioProcessor::paramGainDb, gainSlider);

    bypassAttachment = std::make_unique<ButtonAttachment>(
        audioProcessor.apvts, JuceOneAudioProcessor::paramBypass, bypassButton);

    setSize (320, 220);
}

void JuceOneAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void JuceOneAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (20);

    gainSlider.setBounds (area.removeFromTop (150));
    area.removeFromTop (10);
    bypassButton.setBounds (area.removeFromTop (30));
}
