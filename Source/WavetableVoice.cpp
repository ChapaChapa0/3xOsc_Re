/*
  ==============================================================================

    WavetableVoice.cpp
    Created: 13 Feb 2022 8:13:49pm
    Author:  Chapa

  ==============================================================================
*/

#include "WavetableVoice.h"
#include "WavetableSound.h"

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
    levelMidiNote = velocity * 0.2;
    currentIndex[0] = 0.0f;
    currentIndex[1] = 0.0f;
    currentIndex[2] = 0.0f;
    tailOff = 0.0;

    auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto tableSizeOverSampleRate = (float)NUM_SAMPLES / getSampleRate();
    auto tableDeltaInit = cyclesPerSecond * tableSizeOverSampleRate;

    tableDelta[0] = tableDeltaInit;
    tableDelta[1] = tableDeltaInit;
    tableDelta[2] = tableDeltaInit;

    updateValue();
}

void WavetableVoice::stopNote(float velocity, bool allowTailOff)
{
    if (allowTailOff)
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
        float currentSampleLeft = 0.0f;
        float currentSampleRight = 0.0f;

        for (int i = 0; i < 3; ++i)
        {
            float currentSample_i;

            if (wavetableOsc[i] == 4)
            {
                currentSample_i = (random.nextFloat() - 0.5f) * levelMidiNote * (levelOsc[i] / 100.0f) * ((tailOff > 0.0) ? tailOff : 1.0);
            }
            
            else
            {
                auto index0 = (unsigned int)currentIndex[i];
                auto index1 = index0 == (NUM_SAMPLES - 1) ? (unsigned int)0 : index0 + 1;

                auto frac = currentIndex[i] - (float)index0;

                auto* table = wavetables.getReadPointer(wavetableOsc[i]);
                auto value0 = table[index0];
                auto value1 = table[index1];

                currentSample_i = (float)(value0 + frac * (value1 - value0)) * levelMidiNote * (levelOsc[i] / 100.0f) * ((tailOff > 0.0) ? tailOff : 1.0);

                float updatedTableDelta = tableDelta[i] * pow(10.0, ((double)coarseOsc[i] * 100 + (double)fineOsc[i]) / (2400 * 3.322038403));
                if ((currentIndex[i] += updatedTableDelta) > (float)NUM_SAMPLES)
                    currentIndex[i] -= (float)NUM_SAMPLES;
            }

            float leftOsc_i = abs(panOsc[i] - 100.0f) / 200.0f;
            float rightOsc_i = abs(panOsc[i] + 100.0f) / 200.0f;

            currentSampleLeft += currentSample_i * leftOsc_i;
            currentSampleRight += currentSample_i * rightOsc_i;
        }

        for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
            outputBuffer.addSample(i, startSample, ((i % 2 == 0) ? currentSampleLeft : currentSampleRight)* (masterVolume / 100.0));

        ++startSample;

        if (tailOff > 0.0) 
        {
            tailOff *= 0.99;
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

    coarseOsc[0] = *(params->getRawParameterValue(strs[3]));
    coarseOsc[1] = *(params->getRawParameterValue(strs[4]));
    coarseOsc[2] = *(params->getRawParameterValue(strs[5]));

    fineOsc[0] = *(params->getRawParameterValue(strs[6]));
    fineOsc[1] = *(params->getRawParameterValue(strs[7]));
    fineOsc[2] = *(params->getRawParameterValue(strs[8]));

    panOsc[0] = *(params->getRawParameterValue(strs[9]));
    panOsc[1] = *(params->getRawParameterValue(strs[10]));
    panOsc[2] = *(params->getRawParameterValue(strs[11]));

    //float retval = 0.0;

    //auto* samplesToWrite = wavetable.getWritePointer(0);
    //auto* samplesToRead = allWavetables.getReadPointer(0);

    //for (int p = 0; p < NUM_SAMPLES; p++) 
    //{
    //    retval = samplesToRead[p];
    //    samplesToWrite[p] = retval;
    //}
}

void WavetableVoice::generateWavetables()
{
    wavetables.setSize(4, (int)NUM_SAMPLES);
    wavetables.clear();

    juce::Array<juce::Array<int>> harmonics;
    juce::Array<juce::Array<float>> harmonicsWeights;

    // Sine Table
    harmonics.add({ 1 });
    harmonicsWeights.add({ 1.0f });

    // Triangle Table
    harmonics.add({ 1, 3, 5, 7, 9, 11, 13 });
    harmonicsWeights.add({ 1.0f, 0.111f, 0.04f, 0.02f, 0.012f, 0.0082f, 0.0059f });

    // Square Table
    harmonics.add({ 1, 3, 5, 7, 9, 11, 13 });
    harmonicsWeights.add({ 1.0f, 0.333f, 0.2f, 0.142f, 0.111f, 0.091f, 0.077f });

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