/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WavetableVoice.h"
#include "WavetableSound.h"

#define NUM_VOICES 8

//==============================================================================
_3xOsc_ReAudioProcessor::_3xOsc_ReAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), 
                        parameters(*this, nullptr, juce::Identifier("_3xOsc_Re"),
                           {
                           std::make_unique<juce::AudioParameterFloat>("masterVolume", "Master Volume", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 100.f),

                           std::make_unique<juce::AudioParameterFloat>("levelOsc2", "Level Osc 2", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 50.f),
                           std::make_unique<juce::AudioParameterFloat>("levelOsc3", "Level Osc 3", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 25.f),

                           std::make_unique<juce::AudioParameterInt>("coarseOsc1", "Coarse Pitch Osc 1", 0, 48, 24),
                           std::make_unique<juce::AudioParameterInt>("coarseOsc2", "Coarse Pitch Osc 2", 0, 48, 12),
                           std::make_unique<juce::AudioParameterInt>("coarseOsc3", "Coarse Pitch Osc 3", 0, 48, 0),

                           std::make_unique<juce::AudioParameterInt>("fineOsc1", "Fine Pitch Osc 1", -100, 100, 0),
                           std::make_unique<juce::AudioParameterInt>("fineOsc2", "Fine Pitch Osc 2", -100, 100, 0),
                           std::make_unique<juce::AudioParameterInt>("fineOsc3", "Fine Pitch Osc 3", -100, 100, 0),

                           std::make_unique<juce::AudioParameterFloat>("panOsc1", "Panning Osc 1", juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f), 0.0f),
                           std::make_unique<juce::AudioParameterFloat>("panOsc2", "Panning Osc 2", juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f), 0.0f),
                           std::make_unique<juce::AudioParameterFloat>("panOsc3", "Panning Osc 3", juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f), 0.0f),

                           std::make_unique<juce::AudioParameterInt>("stereoOffsetOsc1", "Stereo Phase Offset Osc 1", -100, 100, 0),
                           std::make_unique<juce::AudioParameterInt>("stereoOffsetOsc2", "Stereo Phase Offset Osc 2", -100, 100, 0),
                           std::make_unique<juce::AudioParameterInt>("stereoOffsetOsc3", "Stereo Phase Offset Osc 3", -100, 100, 0),

                           std::make_unique<juce::AudioParameterInt>("stereoDetuneOsc1", "Stereo Detune Osc 1", -50, 50, 0),
                           std::make_unique<juce::AudioParameterInt>("stereoDetuneOsc2", "Stereo Detune Osc 2", -50, 50, 0),
                           std::make_unique<juce::AudioParameterInt>("stereoDetuneOsc3", "Stereo Detune Osc 3", -50, 50, 0),

                           std::make_unique<juce::AudioParameterBool>("sinOsc1", "Sinus Osc 1", true),
                           std::make_unique<juce::AudioParameterBool>("sinOsc2", "Sinus Osc 2", true),
                           std::make_unique<juce::AudioParameterBool>("sinOsc3", "Sinus Osc 3", true),

                           std::make_unique<juce::AudioParameterBool>("squareOsc1", "Square Osc 1", false),
                           std::make_unique<juce::AudioParameterBool>("squareOsc2", "Square Osc 2", false),
                           std::make_unique<juce::AudioParameterBool>("squareOsc3", "Square Osc 3", false),

                           std::make_unique<juce::AudioParameterBool>("roundsawOsc1", "RoundSaw Osc 1", false),
                           std::make_unique<juce::AudioParameterBool>("roundsawOsc2", "RoundSaw Osc 2", false),
                           std::make_unique<juce::AudioParameterBool>("roundsawOsc3", "RoundSaw Osc 3", false),

                           std::make_unique<juce::AudioParameterBool>("triangleOsc1", "Triangle Osc 1", false),
                           std::make_unique<juce::AudioParameterBool>("triangleOsc2", "Triangle Osc 2", false),
                           std::make_unique<juce::AudioParameterBool>("triangleOsc3", "Triangle Osc 3", false),

                           std::make_unique<juce::AudioParameterBool>("sawtoothOsc1", "Sawtooth Osc 1", false),
                           std::make_unique<juce::AudioParameterBool>("sawtoothOsc2", "Sawtooth Osc 2", false),
                           std::make_unique<juce::AudioParameterBool>("sawtoothOsc3", "Sawtooth Osc 3", false),

                           std::make_unique<juce::AudioParameterBool>("noiseOsc1", "Noise Osc 1", false),
                           std::make_unique<juce::AudioParameterBool>("noiseOsc2", "Noise Osc 2", false),
                           std::make_unique<juce::AudioParameterBool>("noiseOsc3", "Noise Osc 3", false),

                           })
#endif
{
    for (auto i = 0; i < NUM_VOICES; ++i)
        synth.addVoice(new WavetableVoice(parameters));

    setUsingWavetableSound();
}

_3xOsc_ReAudioProcessor::~_3xOsc_ReAudioProcessor()
{
}

void _3xOsc_ReAudioProcessor::setUsingWavetableSound()
{
    synth.clearSounds();
    synth.addSound(new WavetableSound());
}


//==============================================================================
const juce::String _3xOsc_ReAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _3xOsc_ReAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _3xOsc_ReAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _3xOsc_ReAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _3xOsc_ReAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _3xOsc_ReAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _3xOsc_ReAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _3xOsc_ReAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _3xOsc_ReAudioProcessor::getProgramName (int index)
{
    return {};
}

void _3xOsc_ReAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _3xOsc_ReAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);
    keyboardState.reset();
}

void _3xOsc_ReAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _3xOsc_ReAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void _3xOsc_ReAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    int num = buffer.getNumSamples();
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear(i, 0, num);
    keyboardState.processNextMidiBuffer(midiMessages, 0, num, true);
    synth.renderNextBlock(buffer, midiMessages, 0, num);
}

void _3xOsc_ReAudioProcessor::updateValue() {
    for (int i = 0; i < NUM_VOICES; i++) 
    {
        WavetableVoice* voice = (WavetableVoice*)(synth.getVoice(i));
        voice->updateValue();
    }
}

//==============================================================================
bool _3xOsc_ReAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _3xOsc_ReAudioProcessor::createEditor()
{
    return new _3xOsc_ReAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void _3xOsc_ReAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void _3xOsc_ReAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _3xOsc_ReAudioProcessor();
}
