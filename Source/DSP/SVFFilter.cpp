/*
  ==============================================================================

    SVFFilter.cpp
    Created: 22 Jan 2022 12:56:41pm
    Author:  Dylan

  ==============================================================================
*/
#include "SVFFilter.h"

void SVFFilter::prepareToPlay(int numChannels, float sampleRate)
{
    updateSampleRate(sampleRate);
    state1.resize(numChannels);
    state2.resize(numChannels);
    reset();
}

void SVFFilter::reset()
{
    avg1 = 0.0f;
    avg2 = 0.0f;
    cutoffFrequency = 300.0f;
    updateResonance(0.1f);
    updateCoeff();
}

void SVFFilter::processSample(float& sample)
{
    float bandPass = (cutoffCoeff * (sample - avg2) + avg1) * dampingCoeff;
    float v1 = bandPass - avg1;
    avg1 = bandPass + v1;
    float v2 = cutoffCoeff * bandPass;
    sample = v2 + avg2;
    avg2 = sample + v2;
}

float SVFFilter::advanceFilter(float sample)
{
    float bandPass = (cutoffCoeff * (sample - avg2) + avg1) * dampingCoeff;
    float v1 = bandPass - avg1;
    avg1 = bandPass + v1;
    float v2 = cutoffCoeff * bandPass;
    sample = v2 + avg2;
    avg2 = sample + v2;
    return sample;
}

void SVFFilter::processSample(float& sample, int channel)
{
    float bandPass = (cutoffCoeff * (sample - state2[channel]) + state1[channel]) * dampingCoeff;
    float v1 = bandPass - state1[channel];
    state1[channel] = bandPass + v1;
    float v2 = cutoffCoeff * bandPass;
    sample = v2 + state2[channel];
    state2[channel] = sample + v2;
}

void SVFFilter::updateSampleRate(float sampleRate)
{
    SVFFilter::sampleRate = sampleRate;
    sampleTime = 1.0f / sampleRate;
    updateCoeff();
}

void SVFFilter::updateCutoff(float frequency)
{
    float nyquistFreq = sampleRate / 2.0f;
    //limits cutoff to slightly below nyqyuist frequency
    cutoffFrequency = std::min(frequency, nyquistFreq - 100.0f);
    updateCoeff();
}

void SVFFilter::adjustCutoff(float ratio)
{
    float frequency = cutoffFrequency * (ratio + 1.0f);

    float nyquistFreq = sampleRate * 0.5f;
    //limits cutoff to slightly below nyqyuist frequency
    frequency = std::min(frequency, nyquistFreq - 100.0f);
    updateCoeff(frequency);
}

void SVFFilter::updateResonance(float resonance)
{
    SVFFilter::damping = 1.0f - resonance;
    updateDamping();
}

void SVFFilter::processBlock(float* bufferPointer, int numSamples, int channel)
{
    for (int sample = 0; sample < numSamples; ++sample)
    {
        processSample(bufferPointer[sample], channel);
    }
}

void SVFFilter::processBlock(float* bufferPointer, float* controlPointer, float ratio, int numSamples, int channel)
{
    for (int sample = 0; sample < numSamples; ++sample)
    {
        adjustCutoff(controlPointer[sample] * ratio + 1.0f);
        processSample(bufferPointer[sample], channel);
    }
}

void SVFFilter::updateCoeff()
{
    float wd = twoPi * cutoffFrequency;
    float wa = (2.0f / sampleTime) * tan(wd * sampleTime / 2.0f);
    cutoffCoeff = (wa * sampleTime) / 2.0f;
    updateDamping();
}

void SVFFilter::updateCoeff(float frequency)
{
    float wd = twoPi * frequency;
    float wa = (2.0f / sampleTime) * tan(wd * sampleTime / 2.0f);
    cutoffCoeff = (wa * sampleTime) / 2.0f;
    updateDamping();
}

void SVFFilter::updateDamping()
{
    dampingCoeff = 1.0f / (1.0f + (2.0f * damping * cutoffCoeff) + (cutoffCoeff * cutoffCoeff));
}