#ifndef CURRENTDIVIDER_H_INCLUDED
#define CURRENTDIVIDER_H_INCLUDED

#include "wdf.h"

using namespace WaveDigitalFilter;

class CurrentDivider
{
public:
    CurrentDivider()
    {
        F1 = std::make_unique<PolarityInverter> (&Is);
        P1 = std::make_unique<WDFParallel> (F1.get(), &R1);
        P2 = std::make_unique<WDFParallel> (P1.get(), &R2);
        
        O1.connectToNode (P2.get());
    }

    void setResistorValues (float r1, float r2)
    {
        R1.setResistanceValue ((double) r1);
        R2.setResistanceValue ((double) r2);
    }

    inline float processSample (float x)
    {
        Is.setCurrent ((double) x);

        O1.incident (P2->reflected());
        P2->incident (O1.reflected());
        float y = R1.current();

        return y;
    }

private:
    Resistor R1 { 10000.0 };
    Resistor R2 { 10000.0 };
    std::unique_ptr<PolarityInverter> F1;
    ResistiveCurrentSource Is;
    std::unique_ptr<WDFParallel> P1;
    std::unique_ptr<WDFParallel> P2;
    Open O1;
};

#endif // CURRENTDIVIDER_H_INCLUDED
