/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginLook.h"
#include "PageButton.h"

//==============================================================================
_3xOsc_ReAudioProcessorEditor::_3xOsc_ReAudioProcessorEditor (_3xOsc_ReAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts), keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Set customized look
    setLookAndFeel(&threeOscLook);

    // Set the main volume slider
    sliderAttachments[0].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "masterVolume", rotarySliders[0]));
    labels[0].setText("Master Volume", juce::NotificationType::dontSendNotification);
    rotarySliders[0].setTextValueSuffix("%");

    labels[0].attachToComponent(&(rotarySliders[0]), false);
    rotarySliders[0].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rotarySliders[0].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
    rotarySliders[0].setTextBoxIsEditable(false);
    rotarySliders[0].setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    rotarySliders[0].setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
    rotarySliders[0].setBounds(825, 45, 100, 100);
    rotarySliders[0].addListener(this);

    // Set the mix level sliders for oscillator 2 and 3
    for (int i = 1; i < 3; ++i)
    {
        sliderAttachments[i].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "levelOsc" + juce::String(i + 1), rotarySliders[i]));
        labels[i].setText("Mix Level", juce::NotificationType::dontSendNotification);
        rotarySliders[i].setTextValueSuffix("%");

        labels[i].attachToComponent(&(rotarySliders[i]), false);
        rotarySliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        rotarySliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        rotarySliders[i].setTextBoxIsEditable(false);
        rotarySliders[i].setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        rotarySliders[i].setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
        rotarySliders[i].setBounds(825, 185 + i * 140, 100, 100);
        rotarySliders[i].addListener(this);
    }

    // Set the coarse pitch, fine pitch and panning sliders for the 3 oscillators
    for (int i = 0; i < 3; ++i)
    {
        // Coarse pitch
        sliderAttachments[i + 3].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "coarseOsc" + juce::String(i + 1), rotarySliders[i + 3]));
        labels[i + 3].setText("Coarse Pitch", juce::NotificationType::dontSendNotification);
        rotarySliders[i + 3].setTextValueSuffix(" sts");

        labels[i + 3].attachToComponent(&(rotarySliders[i + 3]), false);
        rotarySliders[i + 3].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        rotarySliders[i + 3].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        rotarySliders[i + 3].setTextBoxIsEditable(false);
        rotarySliders[i + 3].setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        rotarySliders[i + 3].setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
        rotarySliders[i + 3].setColour(juce::Slider::thumbColourId, juce::Colours::limegreen);
        rotarySliders[i + 3].setBounds(525, 185 + i * 140, 100, 100);
        rotarySliders[i + 3].addListener(this);

        // Fine pitch
        sliderAttachments[i + 6].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "fineOsc" + juce::String(i + 1), rotarySliders[i + 6]));
        labels[i + 6].setText("Fine Pitch", juce::NotificationType::dontSendNotification);
        rotarySliders[i + 6].setTextValueSuffix(" cts");

        labels[i + 6].attachToComponent(&(rotarySliders[i + 6]), false);
        rotarySliders[i + 6].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        rotarySliders[i + 6].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        rotarySliders[i + 6].setTextBoxIsEditable(false);
        rotarySliders[i + 6].setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        rotarySliders[i + 6].setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
        rotarySliders[i + 6].setColour(juce::Slider::thumbColourId, juce::Colours::limegreen);
        rotarySliders[i + 6].setBounds(625, 185 + i * 140, 100, 100);
        rotarySliders[i + 6].addListener(this);

        // Panning
        sliderAttachments[i + 9].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "panOsc" + juce::String(i + 1), panSliders[i]));
        labels[i + 9].setText("Panning", juce::NotificationType::dontSendNotification);

        labels[i + 9].attachToComponent(&(panSliders[i]), false);
        panSliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        panSliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        panSliders[i].setTextBoxIsEditable(false);
        panSliders[i].setBounds(725, 185 + i * 140, 100, 100);
        panSliders[i].addListener(this);
    }

    // Set stereo detune and stereo phase offset sliders for the 3 oscillators
    for (int i = 0; i < 3; ++i)
    {
        // Stereo phase offset
        sliderAttachments[i + 12].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "stereoOffsetOsc" + juce::String(i + 1), offsetSliders[i]));
        labels[i + 12].setText("Phase Ofs", juce::NotificationType::dontSendNotification);

        labels[i + 12].attachToComponent(&(offsetSliders[i]), false);
        offsetSliders[i].setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        offsetSliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        offsetSliders[i].setTextBoxIsEditable(false);
        offsetSliders[i].setBounds(325, 185 + i * 140, 80, 100);
        offsetSliders[i].addListener(this);

        // Stereo detune
        sliderAttachments[i + 15].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "stereoDetuneOsc" + juce::String(i + 1), detuneSliders[i]));
        labels[i + 15].setText("Detune", juce::NotificationType::dontSendNotification);

        labels[i + 15].attachToComponent(&(detuneSliders[i]), false);
        detuneSliders[i].setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        detuneSliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        detuneSliders[i].setTextBoxIsEditable(false);
        detuneSliders[i].setBounds(425, 185 + i * 140, 80, 100);
        detuneSliders[i].addListener(this);
    }

    // Set the wavetable radio buttons for the 3 oscillators
    auto stringWtArray = juce::StringArray("sin", "square", "roundsaw", "triangle", "sawtooth", "noise");

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            auto parameterId = stringWtArray[j] + "Osc" + juce::String(i + 1);
            buttonAttachments[i * 6 + j].reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, parameterId, wavetableButtons[i * 6 + j]));
            wavetableButtons[i * 6 + j].setWavetable(j);
            wavetableButtons[i * 6 + j].setClickingTogglesState(true);
            wavetableButtons[i * 6 + j].setRadioGroupId(i + 1, juce::NotificationType::sendNotification);
            wavetableButtons[i * 6 + j].addListener(this);
            wavetableButtons[i * 6 + j].setBounds(100 + int(j % 3) * 60 , 175 + int(j / 3) * 60 + i * 140, 50, 50);
        }

        wavetableButtons[i * 6].setToggleState(true, juce::NotificationType::sendNotification);
    }

    // Set minor graphical details
    for (int i = 0; i < 3; ++i)
    {
        oscLabels[i].setText(juce::String(i + 1), juce::NotificationType::dontSendNotification);
        oscLabels[i].setFont(juce::Font(50.0, 1));
        oscLabels[i].setColour(juce::Label::outlineColourId, blazeorange);
        oscLabels[i].setColour(juce::Label::textColourId, blazeorange);
        oscLabels[i].setBounds(25, 205 + i * 140, 50, 50);
    }

    // Set page buttons
    pagesButton[0].setBounds(45, 45, 100, 100);
    pagesButton[0].setRadioGroupId(1000);
    pagesButton[0].onClick = [this] { updateToggleState(&pagesButton[0], "Main");   };

    pagesButton[1].setBounds(145, 45, 100, 100);
    pagesButton[1].setRadioGroupId(1000);
    pagesButton[1].onClick = [this] { updateToggleState(&pagesButton[1], "Second"); };

    // Make visible components of the main page
    makeMainPageVisible();

    // Set size of the main window
    setSize (950, 620);

    setRepaintsOnMouseActivity(true);
}

_3xOsc_ReAudioProcessorEditor::~_3xOsc_ReAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================

void _3xOsc_ReAudioProcessorEditor::makeMainPageVisible()
{

    // Hide Second Page
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        removeChildComponent(i);
    }

    // Make Main Page Visible
    //addAndMakeVisible(pagesButton[0]);
    //addAndMakeVisible(pagesButton[1]);

    for (int i = 0; i < 18; ++i)
    {
        addAndMakeVisible(wavetableButtons[i]);
        addAndMakeVisible(labels[i]);
    }

    for (int i = 0; i < 9; ++i)
    {
        addAndMakeVisible(rotarySliders[i]);
    }

    for (int i = 0; i < 3; ++i)
    {
        addAndMakeVisible(offsetSliders[i]);
        addAndMakeVisible(detuneSliders[i]);
        addAndMakeVisible(panSliders[i]);
        addAndMakeVisible(oscLabels[i]);
    }
}

void _3xOsc_ReAudioProcessorEditor::makeSecondPageVisible()
{

    // Hide Main Page
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        removeChildComponent(i);
    }

    // Make Second Page Visible
    //addAndMakeVisible(pagesButton[0]);
    //addAndMakeVisible(pagesButton[1]);

    addAndMakeVisible(rotarySliders[0]);
    addAndMakeVisible(labels[0]);
}

//==============================================================================
void _3xOsc_ReAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(slategray);

    juce::Rectangle<int> area(0, 0, getWidth(), 160);
    g.setColour(juce::Colours::darkslategrey);
    g.fillRect(area);
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

void _3xOsc_ReAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    button->repaint();
}

void _3xOsc_ReAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
    audioProcessor.updateValue();
}

void _3xOsc_ReAudioProcessorEditor::updateToggleState(juce::Button* button, juce::String name)
{
    auto state = button->getToggleState();
    if (name == "Main" && state)
    {
        //makeMainPageVisible();
    }
    else if (name == "Second" && state)
    {
        //makeSecondPageVisible();
    }
}
