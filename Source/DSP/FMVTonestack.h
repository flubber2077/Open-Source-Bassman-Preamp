/*
  ==============================================================================

    FMVTonestack.h
    Created: 11 Feb 2022 2:01:35pm
    Author:  Dylan

  ==============================================================================
*/

#pragma once
#include <vector>

class FMVTonestack
{
public:
    void updateTreble(float t);
    void updateBass(float b);
    void calcCoeffecients();
    void processSample(float &sample, int channel);
    void processBlock(float *samplePointer, int numSamples, int channel);
    void prepareToPlay(int numChannels, float sampleRate);

private:
    // 2/sampletime
    float c;

    //the literal constants of the components inside the filter. Set up for a fender bassman
    const float c1 = 0.00000000025f;
    const float c2 = 0.000000020f;
    const float c3 = c2;
    const float r1 = 250000.0f;
    const float r2 = 1000000.0f;
    const float r3 = 6800.0f;
    const float r4 = 56000.0f;

    //variable controls, to be rotated from 0-1
    float treble = 1.0f;
    float bass = 1.0f;
    const float mid = 1.0f;

    //the final digital filter coefficients
    float B0;
    float B1;
    float B2;
    float B3;
    float A0;
    float A1;
    float A2;
    float A3;

    //previous inputs and outputs
    std::vector<float> previousInput;
    std::vector<float> previousOutput;
};