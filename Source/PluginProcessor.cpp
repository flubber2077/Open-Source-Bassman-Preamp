/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PanOFlexAudioProcessor::PanOFlexAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
     ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
}

PanOFlexAudioProcessor::~PanOFlexAudioProcessor()
{
}

//==============================================================================
const juce::String PanOFlexAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PanOFlexAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PanOFlexAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PanOFlexAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PanOFlexAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PanOFlexAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PanOFlexAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PanOFlexAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PanOFlexAudioProcessor::getProgramName (int index)
{
    return {};
}

void PanOFlexAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PanOFlexAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    int numChannels = getNumInputChannels();
    miller1.prepareToPlay(numChannels, sampleRate);
    tube1.prepareToPlay(numChannels);
    rcfilter1.prepareToPlay(numChannels, sampleRate);
    volumeControl.prepareToPlay(numChannels, sampleRate);
    miller2.prepareToPlay(numChannels, sampleRate);
    tube2.prepareToPlay(numChannels);
    rcfilter2.prepareToPlay(numChannels, sampleRate);

    //placeholder cutoff values but ballpark accurate/workable
    miller1.updateCutoff(22000.0f);
    rcfilter1.updateCutoff(20.0f);
    volumeControl.updateCutoff(4000.0f);
    volumeControl.updateGain(0.5f);
    miller2.updateCutoff(22000.0f);
    rcfilter2.updateCutoff(20.0f);
}

void PanOFlexAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PanOFlexAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PanOFlexAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto& volume = *apvts.getRawParameterValue("VOLUME");
    volumeControl.updateGain(volume);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();

    //this gain amount should be varied from simulation to better accept a more standard signal level. maybe a pad input at the beginning?jl
    buffer.applyGain(2.0f);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        miller1.processBlock(channelData, numSamples, channel);
        tube1.processBlock(channelData, numSamples, channel);
        rcfilter1.processBlock(channelData, numSamples, channel);
        volumeControl.processBlock(channelData, numSamples, channel);
        miller2.processBlock(channelData, numSamples, channel);
    }
    buffer.applyGain(40.0f);
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        tube2.processBlock(channelData, numSamples, channel);
        rcfilter2.processBlock(channelData, numSamples, channel);
    }
    //this should be a dedicated master volume
    buffer.applyGain(0.5f);

    //gain control compensation. at 1/5 its very slight and at 1/4 it sounds even
    buffer.applyGain(powf(volume, -1.0f / 5.0f));
}

//==============================================================================
bool PanOFlexAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PanOFlexAudioProcessor::createEditor()
{
    return new PanOFlexAudioProcessorEditor (*this);
}

//==============================================================================
void PanOFlexAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PanOFlexAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PanOFlexAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout PanOFlexAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", juce::NormalisableRange<float> { 0.001f, 1.0f, 0.001f, 0.3f }, 0.1f));
    return { params.begin(), params.end() };
}