/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginLook.h"

//==============================================================================
_3xOsc_ReAudioProcessorEditor::_3xOsc_ReAudioProcessorEditor (_3xOsc_ReAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts), keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Set customized look
    setLookAndFeel(&threeOscLook);

    // Set the sliders
    attachments[0].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "masterVolume", sliders[0]));
    labels[0].setText("Master Volume", juce::NotificationType::dontSendNotification);
    sliders[0].setTextValueSuffix("%");

    labels[0].attachToComponent(&(sliders[0]), false);
    sliders[0].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliders[0].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
    sliders[0].setBounds(425, 45, 100, 100);
    sliders[0].addListener(this);
    addAndMakeVisible(sliders[0]);
    addAndMakeVisible(labels[0]);

    for (int i = 1; i < 3; ++i)
    {
        attachments[i].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "levelOsc" + juce::String(i + 1), sliders[i]));
        labels[i].setText("Level Osc "+ juce::String(i + 1), juce::NotificationType::dontSendNotification);
        sliders[i].setTextValueSuffix("%");
    }

    for (int i = 1; i < 3; ++i)
    {
        labels[i].attachToComponent(&(sliders[i]), false);
        sliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        sliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        sliders[i].setBounds(425, 185 + (i * 140), 100, 100);
        sliders[i].addListener(this);
        addAndMakeVisible(sliders[i]);
        addAndMakeVisible(labels[i]);
    }

    for (int i = 0; i < 3; ++i)
    {
        attachments[i + 3].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "coarseOsc" + juce::String(i + 1), sliders[i + 3]));
        labels[i + 3].setText("Coarse Pitch", juce::NotificationType::dontSendNotification);
        sliders[i + 3].setTextValueSuffix(" sts");

        attachments[i + 6].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "fineOsc" + juce::String(i + 1), sliders[i + 6]));
        labels[i + 6].setText("Fine Pitch", juce::NotificationType::dontSendNotification);
        sliders[i + 6].setTextValueSuffix(" cts");

        attachments[i + 9].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "panOsc" + juce::String(i + 1), panSliders[i]));
        labels[i + 9].setText("Panning", juce::NotificationType::dontSendNotification);
    }

    for (int i = 0; i < 3; ++i)
    {
        labels[i + 3].attachToComponent(&(sliders[i + 3]), false);
        sliders[i + 3].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        sliders[i + 3].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        sliders[i + 3].setBounds(125, 185 + (i * 140), 100, 100);
        sliders[i + 3].addListener(this);
        addAndMakeVisible(sliders[i + 3]);
        addAndMakeVisible(labels[i + 3]);

        labels[i + 6].attachToComponent(&(sliders[i + 6]), false);
        sliders[i + 6].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        sliders[i + 6].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        sliders[i + 6].setBounds(225, 185 + (i * 140), 100, 100);
        sliders[i + 6].addListener(this);
        addAndMakeVisible(sliders[i + 6]);
        addAndMakeVisible(labels[i + 6]);

        labels[i + 9].attachToComponent(&(panSliders[i]), false);
        panSliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        panSliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        panSliders[i].setBounds(325, 185 + (i * 140), 100, 100);
        panSliders[i].addListener(this);
        addAndMakeVisible(panSliders[i]);
        addAndMakeVisible(labels[i + 9]);
    }

    // Set size of the main window
    setSize (550, 620);

    setRepaintsOnMouseActivity(true);
}

_3xOsc_ReAudioProcessorEditor::~_3xOsc_ReAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void _3xOsc_ReAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void _3xOsc_ReAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void _3xOsc_ReAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    slider->repaint();
    audioProcessor.updateValue();
}