/*
  ==============================================================================

    BrightVolume.h
    Created: 2 Feb 2022 12:18:15am
    Author:  Dylan

  ==============================================================================
*/

#pragma once
#include "Filter.h"

//mimics a classic volume control that is partially bypassed by capacitor to let highs through

class BrightVolume : public Filter
{
public:
    void processSample(float& sample, int channel) override;
    void updateGain(float gain);
    void processBlock(float* samplePointer, int numSamples, int channel) override;
    void updateSwitch(bool toggle);
private:
    bool brightSwitch;
    float mainGain;
    float highGain;
    const float outputImpedanceRatio = 0.05f; //ratio of output impedance to volume control impedance
};