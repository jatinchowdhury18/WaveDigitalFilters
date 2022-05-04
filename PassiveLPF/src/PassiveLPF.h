#ifndef PASSIVELPF_H_INCLUDED
#define PASSIVELPF_H_INCLUDED

#include <chowdsp_wdf/chowdsp_wdf.h>
#include <juce_dsp/juce_dsp.h>

using namespace chowdsp::wdft;

class PassiveLPF
{
public:
    PassiveLPF() = default;

    void prepare (double sampleRate)
    {
        C1.prepare ((float) sampleRate);
        C2.prepare ((float) sampleRate);
    }

    void reset()
    {
        C1.reset();
        C2.reset();
    }

    void setCutoffFrequency (float cutoff)
    {
        const auto Cap = 5.0e-8f;
        const auto Res = 1.0f / (juce::MathConstants<float>::twoPi * cutoff * Cap);

        R1.setResistanceValue (Res);
        R2.setResistanceValue (Res);
        C1.setCapacitanceValue (Cap);
        C2.setCapacitanceValue (Cap);
    }

    inline float processSample (float x)
    {
        Vs.setVoltage (x);

        Vs.incident (S2.reflected());
        auto y = voltage<float> (C2);
        S2.incident (Vs.reflected());

        return y;
    }

private:
    ResistorT<float> R1 { 1000.0f };
    ResistorT<float> R2 { 1000.0f };

    CapacitorT<float> C1 { 5.0e-8f, 48000.0f };
    CapacitorT<float> C2 { 5.0e-8f, 48000.0f };

    WDFSeriesT<float, decltype (R2), decltype (C2)> S1 { R2, C2 };
    WDFParallelT<float, decltype (C1), decltype (S1)> P1 { C1, S1 };
    WDFSeriesT<float, decltype (R1), decltype (P1)> S2 { R1, P1 };

    IdealVoltageSourceT<float, decltype (S2)> Vs { S2 };
};

#endif //PASSIVELPF_H_INCLUDED
