/*
  ==============================================================================

    WavetableButton.cpp
    Created: 16 Feb 2022 11:47:24am
    Author:  Chapa

  ==============================================================================
*/

#include "WavetableButton.h"

WavetableButton::WavetableButton()
{
}

void WavetableButton::setWavetable(int newIndexWavetable)
{
    indexWavetable = newIndexWavetable;
    setImages(true, true, true, imageOff[indexWavetable], 1.0f, juce::Colours::transparentWhite, juce::Image(), 1.0f, 
        juce::Colours::transparentWhite, imageOn[indexWavetable], 1.0f, juce::Colours::transparentWhite, 0.0f);
}