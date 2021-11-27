#ifndef DIODECLIPPER_H_INCLUDED
#define DIODECLIPPER_H_INCLUDED

#include <JuceHeader.h>

using namespace chowdsp::WDFT;

class DiodeClipper
{
public:
    DiodeClipper() = default;

    void prepare (double sampleRate)
    {
        C1.prepare ((float) sampleRate);
    }

    void reset()
    {
        C1.reset();
    }

    void setCircuitParams (float cutoff)
    {
        constexpr auto Cap = 47.0e-9f;
        const auto Res = 1.0f / (MathConstants<float>::twoPi * cutoff * Cap);

        C1.setCapacitanceValue (Cap);
        R1.setResistanceValue (Res);
    }

    void setCircuitElements (float res, float cap)
    {
        C1.setCapacitanceValue (cap);
        R1.setResistanceValue (res);
    }

    inline float processSample (float x)
    {
        Vs.setVoltage (x);

        dp.incident (P1.reflected());
        auto y = voltage<float> (C1);
        P1.incident (dp.reflected());

        return y;
    }

private:
    ResistorT<float> R1 { 4700.0f };
    ResistiveVoltageSourceT<float> Vs;
    WDFSeriesT<float, decltype (Vs), decltype (R1)> S1 { Vs, R1 };

    CapacitorT<float> C1 { 47.0e-9f };
    WDFParallelT<float, decltype (S1), decltype (C1)> P1 { S1, C1 };

    // GZ34 diode pair
    DiodePairT<float, decltype (P1)> dp { P1, 2.52e-9f };
};

#endif //DIODECLIPPER_H_INCLUDED
