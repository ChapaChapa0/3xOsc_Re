/*
  ==============================================================================

    WavetableVoice.cpp
    Created: 13 Feb 2022 8:13:49pm
    Author:  Chapa

  ==============================================================================
*/

#include "WavetableVoice.h"
#include "WavetableSound.h"
using namespace std;

#define NUM_SAMPLES 512


WavetableVoice::WavetableVoice(juce::AudioProcessorValueTreeState& args) : params(&args) {
    generateWavetables();
}

bool WavetableVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<WavetableSound*> (sound) != nullptr;
}

void WavetableVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    levelMidiNote = velocity * 0.5;
    attackEnvelop = 0.0;
    tailOff = 0.0;

    auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();
    auto samplesPerCycle = 1.0 / cyclesPerSample;

    auto tableSizeOverSampleRate = (float)NUM_SAMPLES / getSampleRate();
    auto tableDeltaInit = cyclesPerSecond * tableSizeOverSampleRate;

    for (int i = 0; i < 3; ++i)
    {
        // Left
        currentIndex[i] = ((stereoOffsetOsc[i] < 0) ? abs(stereoOffsetOsc[i]) : 0.0) / 100.0 * samplesPerCycle;
        tableDelta[i] = tableDeltaInit;

        // Right
        currentIndex[i + 3] = ((stereoOffsetOsc[i] > 0) ? stereoOffsetOsc[i] : 0.0) / 100.0 * samplesPerCycle;
        tableDelta[i + 3] = tableDeltaInit;
    }

    updateValue();
}

void WavetableVoice::stopNote(float velocity, bool allowTailOff)
{
    if (true)
    {
        if (tailOff == 0.0) tailOff = 1.0;
    }
    else
    {
        clearCurrentNote();
    }
}

void WavetableVoice::pitchWheelMoved(int)
{
}

void WavetableVoice::controllerMoved(int, int)
{
}

void WavetableVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    processBlock(outputBuffer, startSample, numSamples);
}

void WavetableVoice::renderNextBlock(juce::AudioBuffer<double>& outputBuffer, int startSample, int numSamples)
{
    processBlock(outputBuffer, startSample, numSamples);
}

template<typename FloatType>
inline void WavetableVoice::processBlock(juce::AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
{
    while (--numSamples >= 0)
    {
        if (attackEnvelop < 1.0)
            attackEnvelop = 1.0 - ((1.0 - attackEnvelop) * 0.99) + 0.001;
        else
            attackEnvelop = 1.0;

        float currentSampleLeft = 0.0f;
        float currentSampleRight = 0.0f;

        for (int i = 0; i < 3; ++i)
        {
            float currentSampleLeft_i;
            float currentSampleRight_i;

            if (wavetableOsc[i] == 5)
            {

                currentSampleLeft_i = (random.nextFloat() - 0.5f) * levelMidiNote * (levelOsc[i] / 100.0f) * attackEnvelop * ((tailOff > 0.0) ? tailOff : 1.0);
                currentSampleRight_i = currentSampleLeft_i;
            }
            
            else
            {
                auto* table = wavetables.getReadPointer(wavetableOsc[i]);

                // Left
                auto indexL0 = (unsigned int)currentIndex[i];
                auto indexL1 = indexL0 == (NUM_SAMPLES - 1) ? (unsigned int)0 : indexL0 + 1;

                auto fracL = currentIndex[i] - (float)indexL0;
                
                auto valueL0 = table[indexL0];
                auto valueL1 = table[indexL1];

                currentSampleLeft_i = (float)(valueL0 + fracL * (valueL1 - valueL0)) * levelMidiNote * (levelOsc[i] / 100.0f) * attackEnvelop * ((tailOff > 0.0) ? tailOff : 1.0);

                float updatedTableDeltaL = tableDelta[i] * pow(10.0, ((coarseOsc[i] - 24.0) * 100.0 + fineOsc[i]) / (1200.0 * 3.322038403));
                if ((currentIndex[i] += updatedTableDeltaL) > (float)NUM_SAMPLES)
                    currentIndex[i] -= (float)NUM_SAMPLES;

                // Right
                auto indexR0 = (unsigned int)currentIndex[i + 3];
                auto indexR1 = indexR0 == (NUM_SAMPLES - 1) ? (unsigned int)0 : indexR0 + 1;

                auto fracR = currentIndex[i + 3] - (float)indexR0;

                auto valueR0 = table[indexR0];
                auto valueR1 = table[indexR1];

                currentSampleRight_i = (float)(valueR0 + fracR * (valueR1 - valueR0)) * levelMidiNote * (levelOsc[i] / 100.0f) * attackEnvelop * ((tailOff > 0.0) ? tailOff : 1.0);

                float updatedTableDeltaR = tableDelta[i] * pow(10.0, ((coarseOsc[i] - 24.0) * 100.0 + fineOsc[i] + stereoDetuneOsc[i]) / (1200.0 * 3.322038403));
                if ((currentIndex[i + 3] += updatedTableDeltaR) > (float)NUM_SAMPLES)
                    currentIndex[i + 3] -= (float)NUM_SAMPLES;
            }

            float leftOsc_i = abs(panOsc[i] - 100.0f) / 200.0f;
            float rightOsc_i = abs(panOsc[i] + 100.0f) / 200.0f;

            currentSampleLeft += currentSampleLeft_i * leftOsc_i;
            currentSampleRight += currentSampleRight_i * rightOsc_i;
        }

        for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
            outputBuffer.addSample(i, startSample, ((i % 2 == 0) ? currentSampleLeft : currentSampleRight)* (masterVolume / 100.0));

        ++startSample;

        if (tailOff > 0.0) 
        {
            tailOff *= 0.995;
            if (tailOff <= 0.005) 
            {
                break;
            }
        }
    }
}

//==============================================================================

void WavetableVoice::updateValue()
{
    masterVolume = *(params->getRawParameterValue(strs[0]));
    levelOsc[1] = *(params->getRawParameterValue(strs[1]));
    levelOsc[2] = *(params->getRawParameterValue(strs[2]));

    for (int i = 0; i < 3; ++i)
    {
        coarseOsc[i] = *(params->getRawParameterValue(strs[i + 3]));
        fineOsc[i] = *(params->getRawParameterValue(strs[i + 6]));
        panOsc[i] = *(params->getRawParameterValue(strs[i + 9]));

        stereoOffsetOsc[i] = *(params->getRawParameterValue(strs[i + 12]));
        stereoDetuneOsc[i] = *(params->getRawParameterValue(strs[i + 15]));
    }

    for (int j = 0; j < 6; ++j)
    {
        auto parameterOsc1 = params->getRawParameterValue(strs[j + 18]);
        auto parameterOsc2 = params->getRawParameterValue(strs[j + 24]);
        auto parameterOsc3 = params->getRawParameterValue(strs[j + 30]);

        if (*parameterOsc1 > 0.5f) wavetableOsc[0] = j;
        if (*parameterOsc2 > 0.5f) wavetableOsc[1] = j;
        if (*parameterOsc3 > 0.5f) wavetableOsc[2] = j;
    }
}

void WavetableVoice::generateWavetables()
{
    wavetables.setSize(5, (int)NUM_SAMPLES);
    wavetables.clear();

    juce::Array<juce::Array<int>> harmonics;
    juce::Array<juce::Array<float>> harmonicsWeights;

    // Sine Table
    harmonics.add({ 1 });
    harmonicsWeights.add({ 1.0f });

    // Square Table
    harmonics.add({ 1, 3, 5, 7, 9, 11, 13 });
    harmonicsWeights.add({ 1.0f, 0.333f, 0.2f, 0.142f, 0.111f, 0.091f, 0.077f });

    // RoundSaw Table
    harmonics.add({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 });
    harmonicsWeights.add({ 1.0f, 0.25f, 0.333f, 0.125f, 0.2f, 0.088f, 0.142f, 0.062f, 0.111f, 0.05f, 0.091f, 0.042f, 0.077f });

    // Triangle Table
    harmonics.add({ 1, 3, 5, 7, 9, 11, 13 });
    harmonicsWeights.add({ 1.0f, 0.111f, 0.04f, 0.02f, 0.012f, 0.0082f, 0.0059f });

    // Sawtooth Table
    harmonics.add({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 });
    harmonicsWeights.add({ 1.0f, 0.5f, 0.333f, 0.25f, 0.2f, 0.166f,  0.143f, 0.125f, 0.111f, 0.1f, 0.091f, 0.083f, 0.077f });

    for (auto k = 0; k < harmonics.size(); ++k)
    {
        auto* samples = wavetables.getWritePointer(k);

        for (auto h = 0; h < harmonics[k].size(); ++h)
        {
            auto angleDelta = juce::MathConstants<double>::twoPi / (double)(NUM_SAMPLES - 1) * harmonics[k][h];
            auto currentAngle = 0.0;

            for (unsigned int i = 0; i < (int)NUM_SAMPLES; ++i)
            {
                auto sample = std::sin(currentAngle);
                samples[i] += (float)sample * harmonicsWeights[k][h];
                currentAngle += angleDelta;
            }
        }
    }
}