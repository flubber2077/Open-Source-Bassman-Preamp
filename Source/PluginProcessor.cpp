/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::String PanOFlexAudioProcessor::paramVolume("Volume");
juce::String PanOFlexAudioProcessor::paramBright("Bright");
juce::String PanOFlexAudioProcessor::paramBass("Bass");
juce::String PanOFlexAudioProcessor::paramMid("Middle");
juce::String PanOFlexAudioProcessor::paramTreble("Treble");
juce::String PanOFlexAudioProcessor::paramOutput("Output");
juce::String PanOFlexAudioProcessor::paramReverb("Reverb");


//==============================================================================
PanOFlexAudioProcessor::PanOFlexAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    apvts.addParameterListener(paramVolume, this);
    apvts.addParameterListener(paramBright, this);
    apvts.addParameterListener(paramBass, this);
    apvts.addParameterListener(paramMid, this);
    apvts.addParameterListener(paramTreble, this);
    apvts.addParameterListener(paramOutput, this);
    apvts.addParameterListener(paramReverb, this);
};

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
    return 0.1f;
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

    oversampling.numChannels = getTotalNumInputChannels();
    oversampling.initProcessing(samplesPerBlock);
    oversampling.reset();
    float oversampledRate = sampleRate * 2.0;

    setLatencySamples(oversampling.getLatencyInSamples());

    miller1.prepareToPlay(numChannels, oversampledRate);
    tube1.prepareToPlay(numChannels);
    rcfilter1.prepareToPlay(numChannels, oversampledRate);
    volumeControl.prepareToPlay(numChannels, oversampledRate);
    miller2.prepareToPlay(numChannels, oversampledRate);
    tube2.prepareToPlay(numChannels);
    rcfilter2.prepareToPlay(numChannels, oversampledRate);
    tonestack.prepareToPlay(numChannels, oversampledRate);
    miller3.prepareToPlay(numChannels, oversampledRate);
    tube3.prepareToPlay(numChannels);
    rcfilter3.prepareToPlay(numChannels, oversampledRate);
    reverb.setSampleRate(sampleRate);

    //placeholder cutoff values but ballpark accurate/workable
    miller1.updateCutoff(22000.0f);
    rcfilter1.updateCutoff(30.0f);
    volumeControl.updateCutoff(10000.0f);
    miller2.updateCutoff(12000.0f);
    rcfilter2.updateCutoff(20.0f);
    tonestack.calcCoeffecients();
    miller3.updateCutoff(24000.0f);
    rcfilter3.updateCutoff(33.0f);

    //make sure values are default
    volumeControl.updateGain(0.1f);
    reverbParams.dryLevel = 1.0f;
    reverbParams.roomSize = 0.3f;
    reverb.setParameters(reverbParams);

    setLatencySamples(oversampling.getLatencyInSamples() +  (3/2)*(sampleRate/oversampledRate));
}

void PanOFlexAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PanOFlexAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == paramVolume)
    {
        newValue *= 0.1f;
        newValue = 0.0100518357f * exp(4.6f * newValue) - 0.009f; //maps from 0 to -60dB
        mVolume = newValue;
        volumeControl.updateGain(mVolume);
        if (newValue > 0.1f)
        {
            volumeCompensation = 1.0f;
        }
        else
        {
            volumeCompensation = powf(10.0f * newValue, -1.0f / 4.0f);
        }
    }
    else if (parameterID == paramBright)
    {
        mBright = newValue;
        volumeControl.updateSwitch(mBright);
    }
    else if (parameterID == paramBass)
    {
        newValue *= 0.1f;
        newValue = 0.01015f * exp(4.6f * newValue) - 0.01015f;
        tonestack.updateBass(newValue);
    }
    else if (parameterID == paramMid)
    {
        newValue *= 0.1f;
        tonestack.updateMid(newValue);
    }
    else if (parameterID == paramTreble)
    {
        newValue *= 0.1f;
        tonestack.updateTreble(newValue);
    }
    else if (parameterID == paramOutput)
    {
        mOutput = powf(10, newValue / 20.0f);
    }
    else if (parameterID == paramReverb)
    {
        newValue *= 0.1f;
        mReverb = newValue * newValue * newValue; //approximation of log response
        mReverb *= 0.3f; //limit max wet signal
        reverbParams.wetLevel = mReverb;
        reverb.setParameters(reverbParams);
    }
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

    //this gain amount should be varied from simulation to better accept a more standard signal level. maybe a pad input at the beginning?
    // Input tube only clips at 6 Vp-p, it does not clip unless someone is doing somethign wrong.
    //buffer.applyGain(2.0f);

    juce::dsp::AudioBlock<float> block(buffer);
    auto oversampledBlock = oversampling.processSamplesUp(block);
    int numSamples = oversampledBlock.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = oversampledBlock.getChannelPointer(channel);

        miller1.processBlock(channelData, numSamples, channel);
        tube1.processBlock(channelData, numSamples, channel);
        rcfilter1.processBlock(channelData, numSamples, channel);
        volumeControl.processBlock(channelData, numSamples, channel);
        miller2.processBlock(channelData, numSamples, channel);
    }
    
    oversampledBlock.multiplyBy(-35.0f);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = oversampledBlock.getChannelPointer(channel);
        tube2.processBlock(channelData, numSamples, channel);
        rcfilter2.processBlock(channelData, numSamples, channel); //technically unnecessary because of tonestack filtering of DC
    }
    
    oversampledBlock.multiplyBy(-35.0f);
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = oversampledBlock.getChannelPointer(channel);
        tonestack.processBlock(channelData, numSamples, channel);
        miller3.processBlock(channelData, numSamples, channel);
        tube3.processBlock(channelData, numSamples, channel);
        rcfilter3.processBlock(channelData, numSamples, channel);
    }
    
    oversampling.processSamplesDown(block);

    //down by 10db, then master control, then gain control compensation. at 1/5 its very slight and at 1/4 it sounds even
    buffer.applyGain(0.2f * mOutput * volumeCompensation);

    reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
}

juce::AudioProcessorValueTreeState& PanOFlexAudioProcessor::getValueTreeState()
{
    return apvts;
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

    params.push_back(std::make_unique<juce::AudioParameterFloat>(paramVolume, TRANS ("Volume"), juce::NormalisableRange<float> { 0.0f, 10.0f, 0.1f, 1.0f }, 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>(paramBright, TRANS ("Bright Switch"), false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(paramBass, TRANS("Bass"), juce::NormalisableRange<float> { 0.0f, 10.0f, 0.1f, 1.0f }, 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(paramMid, TRANS("Middle"), juce::NormalisableRange<float> { 0.0f, 10.0f, 0.1f, 1.0f }, 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(paramTreble, TRANS("Treble"), juce::NormalisableRange<float> { 0.0f, 10.0f, 0.1f, 1.0f }, 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(paramOutput, TRANS ("Output"), juce::NormalisableRange<float> { -60.0f, 0.0f, 0.1f, 1.4f }, -10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(paramReverb, TRANS ("Reverb"), juce::NormalisableRange<float> { 0.0f, 10.0f, 0.1f, 1.0f }, 0.0f));
    return { params.begin(), params.end() };
}