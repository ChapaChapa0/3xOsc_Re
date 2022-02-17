/*
  ==============================================================================

    StereoSlider.h
    Created: 16 Feb 2022 3:55:11pm
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginLook.h"

class StereoOffsetSlider : public juce::Slider
{
public:
    StereoOffsetSlider();
    ~StereoOffsetSlider();
    virtual juce::String getTextFromValue(double) override;

private:
    PluginLook threeOscLook;
};

//==============================================================================

class StereoDetuneSlider : public juce::Slider
{
public:
    StereoDetuneSlider();
    ~StereoDetuneSlider();
    virtual juce::String getTextFromValue(double) override;

private:
    PluginLook threeOscLook;
};