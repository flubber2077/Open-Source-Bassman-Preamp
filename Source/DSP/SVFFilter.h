/*
  ==============================================================================

    SVFFilter.h
    Created: 22 Jan 2022 12:56:41pm
    Author:  Dylan

  ==============================================================================
*/

#pragma once
#include <vector>

//Zavalishin's SVF Filter from VA Filter Design

class SVFFilter
{
public:
    void prepareToPlay(int numChannels, float sampleRate);
    void reset();
    void processSample(float& sample);
    float advanceFilter(float sample);
    void processSample(float& sample, int channel);
    void updateSampleRate(float sampleRate);
    void updateCutoff(float frequency);
    void adjustCutoff(float ratio);
    void updateResonance(float resonance); // 0-1 input range
    void processBlock(float* bufferPointer, int numSamples, int channel);
    void processBlock(float* bufferPointer, float* controlPointer, float ratio, int numSamples, int channel);

private:
    void updateCoeff();
    void updateCoeff(float frequency);
    void updateDamping();
    float cutoffFrequency;
    float cutoffCoeff;
    float damping; //oscillates at 0, do not allow to become 0 unless clipping is implemented
    float dampingCoeff;
    float sampleRate;
    float sampleTime;
    float avg1;
    float avg2;
    const float twoPi = 6.28318530718f;

    //could make this into a single 2d vector, but i think that would be harder to read and not necessarily any faster
    std::vector<float> state1;
    std::vector<float> state2;
};
