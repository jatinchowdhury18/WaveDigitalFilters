#include "TR808OutputFilter.h"

void OutputFilter::prepare (double sampleRate)
{
    c45.prepare ((float) sampleRate);
    c47.prepare ((float) sampleRate);
    c48.prepare ((float) sampleRate);
    c49.prepare ((float) sampleRate);
    c50.prepare ((float) sampleRate);

    I1.setCurrent (-0.169104e-3f);
    I2.setCurrent (50.9002e-3f);
    VnegB2.setVoltage (-15.0f);
}

void OutputFilter::reset()
{
    c45.reset();
    c47.reset();
    c48.reset();
    c49.reset();
    c50.reset();
}

void OutputFilter::setVolume (float vol)
{
    // log taper potentiometer (rough approximation)
    auto l = std::pow (vol, 2.0f);
    l = l < 0.001f ? 0.001f : (l > 0.999f ? 0.999f : l); // clamp

    vr4.setResistanceValue ((1.0f - l) * 100000.0f);
    vr4neg.setResistanceValue (l * 100000.0f);
}

void OutputFilter::setTone (float tone)
{
    // anti-log taper potentiometer (rough approximation)
    auto c = std::pow (tone, 0.5f);
    c = c < 0.001f ? 0.001f : (c > 0.999f ? 0.999f : c); // clamp

    vr5.setResistanceValue ((1.0f - c) * 10000.0f);
}
