/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginLook.h"
#include "PanSlider.h"

//==============================================================================
/**
*/
class _3xOsc_ReAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    _3xOsc_ReAudioProcessorEditor (_3xOsc_ReAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~_3xOsc_ReAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    virtual void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _3xOsc_ReAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::MidiKeyboardComponent keyboardComponent;

    juce::Slider sliders[15];
    PanSlider panSliders[3];
    juce::Label labels[15];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[15];

    PluginLook threeOscLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_3xOsc_ReAudioProcessorEditor)
};
