/*
  ==============================================================================

    BrightVolume.cpp
    Created: 2 Feb 2022 12:18:15am
    Author:  Dylan

  ==============================================================================
*/

#include "BrightVolume.h"


void BrightVolume::processSample(float& sample, int channel)
{
    float input = (sample - state[channel]) * cutoffCoeff;
    float lowPass = input + state[channel];
    state[channel] = lowPass + input;
    float highPass = sample - lowPass;
    sample = mainGain * lowPass + highGain * highPass;
}

void BrightVolume::processBlock(float* samplePointer, int numSamples, int channel)
{
    if (brightSwitch == true) {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            processSample(samplePointer[sample], channel);
        }
    } else {
        for (int sample = 0; sample < numSamples; sample++)
        {
            samplePointer[sample] *= mainGain;
        }
    }
}

void BrightVolume::updateGain(float gain)
{
    mainGain = gain / (1.0f + outputImpedanceRatio);
    highGain = gain / (gain + outputImpedanceRatio);
}

void BrightVolume::updateSwitch(bool toggle)
{
    brightSwitch = toggle;
}