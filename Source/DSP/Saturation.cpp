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
    sample -= 0.1f;
    float sampleDifference = sample - lastSample[channel];
    float currentAntiderivative = antiderivativeClip(sample);
    float output = 0.0f;

    //if statement avoids divide by zero if the last sample is too close to the current
    //the lesser than amount was chosen arbitrarily but works well so far.
    if (fabsf(sampleDifference) < 0.0005f)
    {
        output = clip(0.5f * (sample + lastSample[channel]));
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

float Saturation::clip(float sample)
{
    //must match antiderivativeClip
    return sample / sqrtf(sample * sample + 1.0f);
}

float Saturation::antiderivativeClip(float sample)
{
    //must be the antiderivative of clip, preferably such that F(0) = 0
    return sqrtf(sample * sample + 1.0f) - 1.0f;
}

//alternative clipping functions

//return tanh(sample);
//return (logf(coshf(sample))); //antiderivative of tan(x)