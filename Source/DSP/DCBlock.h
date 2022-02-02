/*
  ==============================================================================

    DCBlock.h
    Created: 2 Feb 2022 12:18:25am
    Author:  Dylan

  ==============================================================================
*/

#pragma once
#include "Filter.h"

class OnePoleHighPass: public Filter
{
public:
    void processSample(float& sample, int channel);
};