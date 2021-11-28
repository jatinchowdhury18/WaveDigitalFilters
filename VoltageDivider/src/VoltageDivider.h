#ifndef VOLTAGEDIVIDER_H_INCLUDED
#define VOLTAGEDIVIDER_H_INCLUDED

#include <JuceHeader.h>

using namespace chowdsp::WDFT;

class VoltageDivider
{
public:
    VoltageDivider() = default;

    void setResistorValues (float r1, float r2)
    {
        R1.setResistanceValue (r1);
        R2.setResistanceValue (r2);
    }

    inline float processSample (float x)
    {
        Vs.setVoltage (x);

        Vs.incident (I1.reflected());
        I1.incident (Vs.reflected());

        return voltage<float> (R2);
    }

private:
    ResistorT<float> R1 { 10000.0f };
    ResistorT<float> R2 { 10000.0f };

    WDFSeriesT<float, decltype (R1), decltype (R2)> S1 { R1, R2 };
    PolarityInverterT<float, decltype (S1)> I1 { S1 };

    IdealVoltageSourceT<float, decltype (I1)> Vs { I1 };
};

#endif //VOLTAGEDIVIDER_H_INCLUDED
