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

void BrightVolume::updateGain(float gain)
{
    mainGain = gain / (1.0f + outputImpedanceRatio);
    highGain = gain / (gain + outputImpedanceRatio);
}