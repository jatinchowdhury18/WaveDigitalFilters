#ifndef VOLTAGEDIVIDER_H_INCLUDED
#define VOLTAGEDIVIDER_H_INCLUDED

#include "wdf.h"

using namespace WaveDigitalFilter;

class VoltageDivider
{
public:
    VoltageDivider() :
        R1 (10000.0),
        R2 (10000.0)
    {
        S1.reset (new WDFSeries (&R1, &R2));
        I1.reset (new PolarityInverter (S1.get()));
        
        I1->connectToNode (&Vs);
    }

    void setResistorValues (float r1, float r2)
    {
        R1.setResistanceValue ((double) r1);
        R2.setResistanceValue ((double) r2);
    }

    inline float processSample (float x)
    {
        Vs.setVoltage ((double) x);

        Vs.incident (I1->reflected());
        I1->incident (Vs.reflected());
        float y = R2.voltage();

        return (float) y;
    }

private:
    Resistor R1;
    Resistor R2;
    std::unique_ptr<PolarityInverter> I1;
    IdealVoltageSource Vs;
    std::unique_ptr<WDFSeries> S1;
};

#endif //VOLTAGEDIVIDER_H_INCLUDED
