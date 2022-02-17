/*
  ==============================================================================

    StereoSlider.cpp
    Created: 16 Feb 2022 3:55:11pm
    Author:  Chapa

  ==============================================================================
*/

#include "StereoSlider.h"
#include "PluginLook.h"

StereoOffsetSlider::StereoOffsetSlider()
{
    setLookAndFeel(&threeOscLook);
    setColour(juce::Slider::thumbColourId, juce::Colours::red);
}

StereoOffsetSlider::~StereoOffsetSlider()
{
    setLookAndFeel(nullptr);
}

juce::String StereoOffsetSlider::getTextFromValue(double value)
{
    return juce::String(value) + "%";
}

//==============================================================================

StereoDetuneSlider::StereoDetuneSlider()
{
    setLookAndFeel(&threeOscLook);
    setColour(juce::Slider::thumbColourId, juce::Colours::red);
}

StereoDetuneSlider::~StereoDetuneSlider()
{
    setLookAndFeel(nullptr);
}

juce::String StereoDetuneSlider::getTextFromValue(double value)
{
    juce::String detuneString;
    if (value > 0)
    {
        detuneString = "+" + juce::String(value) + " cts";
    }
    else
    {
        detuneString = juce::String(value) + " cts";
    }
    return detuneString;
}