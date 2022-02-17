/*
  ==============================================================================

    PluginLook.cpp
    Created: 13 Feb 2022 8:10:14pm
    Author:  Chapa

  ==============================================================================
*/

#include "PluginLook.h"
#include "PanSlider.h"
#include "StereoSlider.h"

PluginLook::PluginLook()
{
    setColour(juce::Slider::thumbColourId, blazeorange);
    setColour(juce::Label::textColourId, juce::Colours::black);

    setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
}