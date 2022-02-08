/*
  ==============================================================================

    HighEQControl.h
    Created: 7 Feb 2022 4:24:22pm
    Author:  Dylan

  ==============================================================================
*/

#pragma once
#include "Filter.h"

class HighEQ : public Filter
{
public:
    void processSample(float& sample, int channel) override;
    void updateGain(float gain);
    void processBlock(float* samplePointer, int numSamples, int channel) override;
    void adjustCutoff(float ratio);
private:
    float mainGain = 0.1f;
    float highGain;
};