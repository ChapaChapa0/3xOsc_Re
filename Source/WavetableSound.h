/*
  ==============================================================================

    WavetableSound.h
    Created: 13 Feb 2022 8:12:57pm
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

struct WavetableSound : public juce::SynthesiserSound
{
    WavetableSound();

    bool appliesToNote(int) override;
    bool appliesToChannel(int) override;
};

