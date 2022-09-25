/*
  ==============================================================================

    FMVTonestack.cpp
    Created: 11 Feb 2022 2:01:35pm
    Author:  Dylan

  ==============================================================================
*/

#include "FMVTonestack.h"
#include <algorithm>


void FMVTonestack::updateTreble(float t)
{
    treble = t;
    calcCoeffecients();
}

void FMVTonestack::updateMid(float m)
{
    mid = m;
    calcCoeffecients();
}

void FMVTonestack::updateBass(float b)
{
    bass = b;
    calcCoeffecients();
}

void FMVTonestack::calcCoeffecients()
{
    //calculate analog coefficients. Entering these was not fun
    float b1 = treble * c1 * r1 + mid * c3 * r3 + bass * (c1 * r2 + c2 * r2) + c1 * r3 + c2 * r3;

    float b2 = treble * (c1 * c2 * r1 * r4 + c1 * c3 * r1 * r4) - mid * mid * (c1 * c3 * r3 * r3 + c2 * c3 * r3 * r3)
        + mid * (c1 * c3 * r1 * r3 + c1 * c3 * r3 * r3 + c2 * c3 * r3 * r3)
        + bass * (c1 * c2 * r1 * r2 + c1 * c2 * r2 * r4 + c1 * c3 * r2 * r4)
        + bass * mid * (c1 * c3 * r2 * r3 + c2 * c3 * r2 * r3)
        + (c1 * c2 * r1 * r3 + c1 * c2 * r3 * r4 + c1 * c3 * r3 * r4);

    float b3 = bass * mid * (c1 * c2 * c3 * r1 * r2 * r3 + c1 * c2 * c3 * r2 * r3 * r4)
        - mid * mid * (c1 * c2 * c3 * r1 * r3 * r3 + c1 * c2 * c3 * r3 * r3 * r4)
        + mid * (c1 * c2 * c3 * r1 * r3 * r3 + c1 * c2 * c3 * r3 * r3 * r4)
        + treble * c1 * c2 * c3 * r1 * r3 * r4 - treble * mid * c1 * c2 * c3 * r1 * r3 * r4
        + treble * bass * c1 * c2 * c3 * r1 * r2 * r4;

    float a0 = 1.0f;

    float a1 = (c1 * r1 + c1 * c3 + c2 * r3 + c2 * r4 + c3 * r4)
        + mid * c3 * r3 + bass * (c1 * r2 + c2 * r2);

    float a2 = mid * (c1 * c3 * r1 * r3 - c2 * c3 * r3 * r4 + c1 * c3 * r3 * r3
        + c2 * c3 * r3 * r3) + bass * mid * (c1 * c3 * r2 * r3 + c2 * c3 * r2 * r3)
        - mid * mid * (c1 * c3 * r3 * r3 + c2 * c3 * r3 * r3) + bass * (c1 * c2 * r2 * r4
        + c1 * c2 * r1 * r2 + c1 * c3 * r2 * r4 + c2 * c3 * r2 * r4)
        + (c1 * c2 * r1 * r4 + c1 * c3 * r1 * r4 + c1 * c2 * r3 * r4
        + c1 * c2 * r1 * r3 + c1 * c3 * r3 * r4 + c2 * c3 * r3 * r4);

    float a3 = bass * mid * (c1 * c2 * c3 * r1 * r3 * r3 + c1 * c2 * c3 * r3 * r3 * r4)
        - mid * mid * (c1 * c2 * c3 * r1 * r3 * r3 + c1 * c2 * c3 * r3 * r3 * r4)
        + mid * (c1 * c2 * c3 * r3 * r3 * r4 + c1 * c2 * c3 * r1 * r3 * r3
            - c1 * c2 * c3 * r1 * r3 * r4) + bass * c1 * c2 * c3 * r1 * r2 * r4
        + c1 * c2 * c3 * r1 * r3 * r4;

    //use analog filter coefficients to get digital filter coefficients
    B0 = -b1 * c - b2 * c * c - b3 * c * c * c;
    B1 = -b1 * c + b2 * c * c + b3 * c * c * c * 3.0f;
    B2 =  b1 * c + b2 * c * c - b3 * c * c * c * 3.0f;
    B3 =  b1 * c - b2 * c * c + b3 * c * c * c;
    A0 = -a0 - a1 * c - a2 * c * c - a3 * c * c * c;
    A1 = -3.0f * a0 - a1 * c + a2 * c * c + 3.0f * a3 * c * c * c;
    A2 = -3.0f * a0 + a1 * c + a2 * c * c - 3.0f * a3 * c * c * c;
    A3 = -a0 + a1 * c - a2 * c * c + a3 * c * c * c;
}

void FMVTonestack::processSample(float& sample, int channel)
{
    int channelOffset = channel * 4;
    float currentSample = sample;

    sample = (1.0f / A0) * (B0 * sample + B1 * previousInput[0 + channelOffset] + B2 * previousInput[1 + channelOffset] + B3 * previousInput[2 + channelOffset]
        - A1 * previousOutput[0 + channelOffset] - A2 * previousOutput[1 + channelOffset] - A3 * previousOutput[2 + channelOffset]);

//advance the previous sample states. Definitely a cleaner way to do all of this, but this is such a specific use case the lack of modularity is alright. I believe this should vectorize anyway but I dont actually know.
    previousInput[2 + channelOffset] = previousInput[1 + channelOffset];
    previousInput[1 + channelOffset] = previousInput[0 + channelOffset];
    previousInput[0 + channelOffset] = currentSample;

    previousOutput[2 + channelOffset] = previousOutput[1 + channelOffset];
    previousOutput[1 + channelOffset] = previousOutput[0 + channelOffset];
    previousOutput[0 + channelOffset] = sample;
}

void FMVTonestack::processBlock(float* samplePointer, int numSamples, int channel)
{
    for (int sample = 0; sample < numSamples; sample++)
    {
        processSample(samplePointer[sample], channel);
    }
}

void FMVTonestack::prepareToPlay(int numChannels, float sampleRate)
{
    c = 2.0f * sampleRate;
    previousInput.resize(4 * numChannels);
    previousOutput.resize(4 * numChannels);
}
