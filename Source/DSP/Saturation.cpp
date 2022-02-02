/*
  ==============================================================================
    saturation.cpp
    Created: 26 Jan 2022 10:20:07am
    Author:  Dylan Jordan
  ==============================================================================
*/

#include "saturation.h"

void Saturation::prepareToPlay(int numChannels)
{
    lastSample.resize(numChannels);
    lastAntiderivative.resize(numChannels);
}

void Saturation::processSample(float& sample, int channel)
{
    float sampleDifference = sample - lastSample[channel];
    float currentAntiderivative = antiderivativeClip(sample);
    float output = 0.0f;

    //if statement avoids divide by zero if the last sample is too close to the current
    //the lesser than amount was chosen arbitrarily but works well so far.
    if (sampleDifference < 0.00000001f)
    {
        output = clip((sample + lastSample[channel]) / 2.0f);
    }
    else {
        output = (currentAntiderivative - lastAntiderivative[channel]) / (sampleDifference);
    }

    lastAntiderivative[channel] = currentAntiderivative;
    lastSample[channel] = sample;
    sample = output;
}

void Saturation::processBlock(float* samplePointer, int numSamples, int channel)
{
        for (int sample = 0; sample < numSamples; ++sample)
        {
            processSample(samplePointer[sample], channel);
        }
}

void Saturation::processBlock(juce::AudioBuffer< float >& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* bufferPointer = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            processSample(bufferPointer[sample], channel);
        }
    }
}

float Saturation::clip(float sample)
{
    //must match antiderivativeClip
    return tanh(sample);
}

float Saturation::antiderivativeClip(float sample)
{
    //must be the antiderivative of clip, preferably such that F(0) = 0
    return (logf(coshf(sample))); //antiderivative of tan(x)
}

//alternative clipping functions
//tan(x)
//sample = sample/(1+fabsf(sample));
/*if (fabsf(sample) < 1.0f)
 {
 sample = 0.5f *(3.0f * sample - sample * sample * sample);
 } else {
 sample = copysignf(1.0f, sample);
 }*/