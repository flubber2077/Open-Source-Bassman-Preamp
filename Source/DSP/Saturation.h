/*
  ==============================================================================
    saturation.h
    Created: 26 Jan 2022 10:20:07am
    Author:  Dylan Jordan
  ==============================================================================
*/

#pragma once
#include <cmath>
#include <vector>

//Implements an anti-aliasing clipping scheme from REDUCING THE ALIASING OF NONLINEAR WAVESHAPING USING
//CONTINUOUS-TIME CONVOLUTION. Should be implemented with at least 2x oversampling over the audible range.

class Saturation
{
public:
    void prepareToPlay(int channel);
    void processSample(float& sample, int channel);
    void processBlock(float* sample, int numSamples, int channel);

    float clip(float sample);
    float antiderivativeClip(float sample);

private:
    std::vector<float> lastSample;
    std::vector<float> lastAntiderivative;
};