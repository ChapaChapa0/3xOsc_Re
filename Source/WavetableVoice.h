/*
  ==============================================================================

    WavetableVoice.h
    Created: 13 Feb 2022 8:13:49pm
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

#define NUM_SAMPLES 512

struct WavetableVoice : public juce::SynthesiserVoice
{
    WavetableVoice(juce::AudioProcessorValueTreeState&);

    bool canPlaySound(juce::SynthesiserSound*) override;
    void startNote(int, float, juce::SynthesiserSound*, int) override;
    void stopNote(float, bool) override;
    void pitchWheelMoved(int) override;
    void controllerMoved(int, int) override;

    void renderNextBlock(juce::AudioBuffer<float>&, int, int) override;
    void renderNextBlock(juce::AudioBuffer<double>&, int, int) override;

    //==============================================================================

    void updateValue();
    void generateWavetables();

private:
    template <typename FloatType>
    void processBlock(juce::AudioBuffer<FloatType>&, int, int);
    juce::StringRef strs[15] = 
    {
        juce::StringRef("masterVolume"),
        juce::StringRef("levelOsc2"),
        juce::StringRef("levelOsc3"),

        juce::StringRef("coarseOsc1"),
        juce::StringRef("coarseOsc2"),
        juce::StringRef("coarseOsc3"),

        juce::StringRef("fineOsc1"),
        juce::StringRef("fineOsc2"),
        juce::StringRef("fineOsc3"),

        juce::StringRef("panOsc1"),
        juce::StringRef("panOsc2"),
        juce::StringRef("panOsc3"),

        juce::StringRef("wavetableOsc1"),
        juce::StringRef("wavetableOsc2"),
        juce::StringRef("wavetableOsc3"),
    };

    juce::Random random;
    float currentIndex[3] = { 0.0f, 0.0f, 0.0f };
    float tableDelta[3] = { 0.0f, 0.0f, 0.0f };

    float masterVolume = 100.0f;
    float levelOsc[3] = { 100.0f, 50.0f, 25.0f };
    float coarseOsc[3] = { 24.0f, 12.0f, 0.0f };
    float fineOsc[3] = { 0.0f, 0.0f, 0.0f };
    float panOsc[3] = { 0.0f, 0.0f, 0.0f };
    int wavetableOsc[3] = { 0, 0, 0 };

    double levelMidiNote = 0.0, tailOff = 0.0;
    juce::AudioProcessorValueTreeState* params;

    bool noise = false;
    juce::AudioSampleBuffer wavetables;
};
