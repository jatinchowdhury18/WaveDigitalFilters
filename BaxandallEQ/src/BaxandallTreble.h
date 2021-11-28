#ifndef BAXANDALLTREBLE_H_INCLUDED
#define BAXANDALLTREBLE_H_INCLUDED

#include <JuceHeader.h>

using namespace chowdsp::WDFT;

class BaxandallTreble
{
public:
    BaxandallTreble() = default;

    void prepare (double sampleRate)
    {
        C3.prepare ((float) sampleRate);
        C4.prepare ((float) sampleRate);
    }

    void reset()
    {
        C3.reset();
        C4.reset();
    }

    void setTreble (float treble)
    {
        treble = jlimit (0.001f, 0.999f, treble);
        VR2a.setResistanceValue (100000.0f * (1.0f - treble));
        VR2b.setResistanceValue (100000.0f * treble);
    }

    inline float processSample (float x)
    {
        Vin.setVoltage (x);

        Vin.incident (S3.reflected());
        S3.incident (Vin.reflected());

        return voltage<float> (C4) + voltage<float> (VR2b);
    }

private:
    ResistorT<float> VR2a { 100000.0f };
    ResistorT<float> VR2b { 100000.0f };

    CapacitorT<float> C3 { 2.2e-9f };
    CapacitorT<float> C4 { 22.0e-9f };

    WDFSeriesT<float, decltype (VR2b), decltype (C4)> S1 { VR2b, C4 };
    WDFSeriesT<float, decltype (S1), decltype (VR2a)> S2 { S1, VR2a };
    WDFSeriesT<float, decltype (C3), decltype (S2)> S3 { C3, S2 };

    IdealVoltageSourceT<float, decltype (S3)> Vin { S3 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaxandallTreble)
};

#endif // BAXANDALLTREBLE_H_INCLUDED
