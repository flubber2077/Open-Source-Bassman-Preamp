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
    void processSample(float& sample, int channel);
    void updateGain(float gain);
private:
    float mainGain;
    float highGain;
    float outputImpedanceRatio; //ratio of output impedance to volume control impedance
};