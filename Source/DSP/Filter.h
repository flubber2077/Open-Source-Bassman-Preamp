/*
  ==============================================================================
    filter.h
    Created: 19 Jan 2022 12:35:36pm
    Author:  Dylan
  ==============================================================================
*/

#pragma once

#include <vector>
#include <cmath>

//Zavalishin's 1 pole lowpass filter (direct form II trapezoidal filter) from VA Filter Design

class Filter
{
public:
    void prepareToPlay(int numChannels, float sampleRate);
    void reset();
    void processSample(float& sample);
    float advanceFilter(float sample);
    virtual void processSample(float& sample, int channel);
    void updateSampleRate(float sampleRate);
    void updateCutoff(float frequency);
    void updateTimeConstant(float seconds);
    virtual void processBlock(float* sample, int numSamples, int channel);

protected:
    const float twoPi = 6.2831853f;
    void updateCutoff();
    float cutoffFrequency;
    float cutoffCoeff;
    float keyboardTracking;
    float sampleTime;
    float avg;

    std::vector<float> state;
};
