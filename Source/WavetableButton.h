/*
  ==============================================================================

    WavetableButton.h
    Created: 16 Feb 2022 11:47:24am
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class WavetableButton : public juce::ImageButton
{
public:
    WavetableButton();
    void setWavetable(int);

    juce::Image imageOn[6] =
    {
        juce::ImageCache::getFromMemory(BinaryData::WT_sinus_on_png, BinaryData::WT_sinus_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_square_on_png, BinaryData::WT_square_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_roundsaw_on_png, BinaryData::WT_roundsaw_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_triangle_on_png, BinaryData::WT_triangle_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_sawtooth_on_png, BinaryData::WT_sawtooth_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_noise_on_png, BinaryData::WT_noise_on_pngSize)
    };

    juce::Image imageOff[6] =
    {
        juce::ImageCache::getFromMemory(BinaryData::WT_sinus_off_png, BinaryData::WT_sinus_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_square_off_png, BinaryData::WT_square_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_roundsaw_off_png, BinaryData::WT_roundsaw_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_triangle_off_png, BinaryData::WT_triangle_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_sawtooth_off_png, BinaryData::WT_sawtooth_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_noise_off_png, BinaryData::WT_noise_off_pngSize)
    };

private:
    int indexWavetable = 0;
};