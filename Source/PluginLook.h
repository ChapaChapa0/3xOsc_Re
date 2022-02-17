/*
  ==============================================================================

    PluginLook.h
    Created: 13 Feb 2022 8:10:14pm
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class PluginLook : public juce::LookAndFeel_V4
{
public:
    PluginLook();

    juce::Colour blazeorange = juce::Colour(255, 130, 0); // blaze orange : #FF8200
};
