#ifndef PULSESHAPER_H_INCLUDED
#define PULSESHAPER_H_INCLUDED

#include <chowdsp_wdf/chowdsp_wdf.h>

using namespace chowdsp::wdft;

class PulseShaper
{
public:
    PulseShaper() = default;

    void prepare (double sampleRate)
    {
        c40.prepare ((float) sampleRate);
    }

    void reset()
    {
        c40.reset();
    }

    inline float processSample (float x)
    {
        Vs.setVoltage (x);

        d53.incident (P2.reflected());
        auto y = voltage<float> (r162);
        P2.incident (d53.reflected());

        return y;
    }

private:
    ResistiveVoltageSourceT<float> Vs;

    ResistorT<float> r162 { 4700.0f };
    ResistorT<float> r163 { 100000.0f };
    CapacitorAlphaT<float> c40 { 0.015e-6f, 48000.0f, 0.029f };

    WDFParallelT<float, decltype (c40), decltype (r163)> P1 { c40, r163 };
    WDFSeriesT<float, decltype (Vs), decltype (P1)> S1 { Vs, P1 };
    PolarityInverterT<float, decltype (r162)> I1 { r162 };
    WDFParallelT<float, decltype (I1), decltype (S1)> P2 { I1, S1 };

    DiodeT<float, decltype (P2)> d53 { P2, 2.52e-9f }; // 1N4148 diode
};

#endif // PULSESHAPER_H_INCLUDED
