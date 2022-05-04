#ifndef CURRENTDIVIDER_H_INCLUDED
#define CURRENTDIVIDER_H_INCLUDED

#include <chowdsp_wdf/chowdsp_wdf.h>

using namespace chowdsp::wdft;

class CurrentDivider
{
public:
    CurrentDivider() = default;

    void setResistorValues (float r1, float r2)
    {
        R1.setResistanceValue (r1);
        R2.setResistanceValue (r2);
    }

    inline float processSample (float x)
    {
        Is.setCurrent (x);

        Is.incident (I1.reflected());
        I1.incident (Is.reflected());

        return current<float> (R1);
    }

private:
    ResistorT<float> R1 { 10000.0f };
    ResistorT<float> R2 { 10000.0f };

    WDFParallelT<float, decltype (R1), decltype (R2)> P1 { R1, R2 };
    PolarityInverterT<float, decltype (P1)> I1 { P1 };

    IdealCurrentSourceT<float, decltype (I1)> Is { I1 };
};

#endif // CURRENTDIVIDER_H_INCLUDED
