#include "SallenKeyLPFWDF.h"

void SallenKeyLPF::prepare (double sampleRate)
{
    C1.prepare ((float) sampleRate);
    C2.prepare ((float) sampleRate);
}

void SallenKeyLPF::reset()
{
    C1.reset();
    C2.reset();
}

void SallenKeyLPF::setParams (float freqHz, float qVal)
{
    // geometric mean of resistors:
    auto Rval = 1.0f / (capVal * juce::MathConstants<float>::twoPi * freqHz);

    // ratio of resistors:
    qVal = juce::jlimit (0.01f, capRatio * 0.5f, qVal);
    auto Rratio = 0.64174f; // (capRatio + std::sqrt (capRatio * capRatio - 4.0f * qVal * qVal)) / (2.0f * qVal);

    R1.setResistanceValue (Rval * Rratio);
    R2.setResistanceValue (Rval / Rratio);
}
