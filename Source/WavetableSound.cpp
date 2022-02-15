/*
  ==============================================================================

    WavetableSound.cpp
    Created: 13 Feb 2022 8:12:57pm
    Author:  Chapa

  ==============================================================================
*/

#include "WavetableSound.h"


WavetableSound::WavetableSound() {}

bool WavetableSound::appliesToNote(int)
{
    return true;
}

bool WavetableSound::appliesToChannel(int)
{
    return true;
}
