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
#include "StereoSlider.h"
#include "WavetableButton.h"

//==============================================================================
/**
*/
class _3xOsc_ReAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::Button::Listener, juce::Timer
{
public:
    _3xOsc_ReAudioProcessorEditor (_3xOsc_ReAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~_3xOsc_ReAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void buttonClicked(juce::Button* button) override;
    virtual void buttonStateChanged(juce::Button* button) override;
    void makeMainPageVisible();

    juce::Colour blazeorange = juce::Colour(255, 130, 0);
    juce::Colour coolgray = juce:: Colour(140, 146, 172);
    juce::Colour slategray = juce::Colour(118, 134, 146);

private:
    void timerCallback() override;

    _3xOsc_ReAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::MidiKeyboardComponent keyboardComponent;

    WavetableButton wavetableButtons[18];

    juce::Slider rotarySliders[9];
    PanSlider panSliders[3];
    
    StereoOffsetSlider offsetSliders[3];
    StereoDetuneSlider detuneSliders[3];

    juce::Label labels[18];
    juce::Label oscLabels[3];

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments[18];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments[18];

    PluginLook threeOscLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_3xOsc_ReAudioProcessorEditor)
};
