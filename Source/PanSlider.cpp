/*
  ==============================================================================

    PanSlider.cpp
    Created: 13 Feb 2022 11:43:56pm
    Author:  Chapa

  ==============================================================================
*/

#include "PanSlider.h"

PanSlider::PanSlider()
{
}

juce::String PanSlider::getTextFromValue(double value)
{
    juce::String panString;
    if (value < 0)
    {
        panString = juce::String(abs(value)) + "% left";
    }
    else if (value > 0)
    {
        panString = juce::String(abs(value)) + "% right";
    }
    else
    {
        panString = "Centered";
    }
    return panString;
}