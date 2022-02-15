/*
  ==============================================================================

    PanSlider.h
    Created: 13 Feb 2022 11:43:56pm
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class PanSlider : public juce::Slider
{
public:
    PanSlider();
    virtual juce::String getTextFromValue(double) override;
};