/*
  ==============================================================================

    DCBlock.cpp
    Created: 2 Feb 2022 12:18:25am
    Author:  Dylan

  ==============================================================================
*/

#include "DCBlock.h"

void OnePoleHighPass::processSample(float& sample, int channel)
{
    float input = (sample - state[channel]) * cutoffCoeff;
    float lowpass = input + state[channel];
    state[channel] = lowpass + input;
    sample -= lowpass;
}