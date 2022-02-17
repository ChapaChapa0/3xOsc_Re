/*
  ==============================================================================

    PanSlider.h
    Created: 13 Feb 2022 11:43:56pm
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginLook.h"

class PanSlider : public juce::Slider
{
public:
    PanSlider();
    ~PanSlider();
    virtual juce::String getTextFromValue(double) override;

    juce::Colour blazeorange = juce::Colour(255, 130, 0);

private:
    PluginLook threeOscLook;
};