/*
  ==============================================================================

    HighEQControl.cpp
    Created: 7 Feb 2022 4:24:22pm
    Author:  Dylan

  ==============================================================================
*/

#include "HighEQControl.h"

void HighEQ::processSample(float& sample, int channel)
{
    float input = (sample - state[channel]) * cutoffCoeff;
    float lowPass = input + state[channel];
    state[channel] = lowPass + input;
    float highPass = sample - lowPass;
    sample = mainGain * lowPass + highGain * highPass;
}

void HighEQ::processBlock(float* samplePointer, int numSamples, int channel)
{
        for (int sample = 0; sample < numSamples; ++sample)
        {
            processSample(samplePointer[sample], channel);
        }
}

void HighEQ::updateGain(float gain)
{
    highGain = gain;
    adjustCutoff(gain);
}

void HighEQ::adjustCutoff(float ratio)
{
    float frequency = cutoffFrequency * (ratio * ratio * 11.0f + 1.0f);
}