#ifndef TR808OUTPUTFILTER_H_INCLUDED
#define TR808OUTPUTFILTER_H_INCLUDED

#include <JuceHeader.h>

using namespace chowdsp::WDFT;

class OutputFilter
{
public:
    OutputFilter() = default;

    void prepare (double sampleRate);
    void reset();

    void setVolume (float vol);
    void setTone (float tone);

    inline float processSample (float x)
    {
        Vbtr.setVoltage (x);

        I2.incident (inv4.reflected());
        auto voltageOut = voltage<float> (r174);
        inv4.incident (I2.reflected());

        return voltageOut;
    }

private:
    ResistorT<float> r172 { 10000.0f };
    ResistorT<float> r174 { 100000.0f };
    ResistorT<float> r177 { 82000.0f };

    // potentiometers
    ResistorT<float> vr4 { 100000.0f }; // (1-l)VR_4, VR_4 goes up to 100k
    ResistorT<float> vr4neg { 0.1f }; // other side of vr4
    ResistorT<float> vr5 { 10000.0f }; // (1-c)VR_5, VR_5 goes up to 10k

    ResistiveVoltageSourceT<float> Vbtr { 220.0f };
    ResistiveCurrentSourceT<float> I1 { 100000.0f };
    ResistiveVoltageSourceT<float> VnegB2 { 6800.0f };

    CapacitorT<float> c45 { 0.1e-6f };
    CapacitorT<float> c47 { 0.47e-6f };
    CapacitorT<float> c48 { 220.0e-9f };
    CapacitorT<float> c49 { 0.47e-6f };
    CapacitorT<float> c50 { 1.0e-6f };

    PolarityInverterT<float, decltype (Vbtr)> inv1 { Vbtr };
    WDFParallelT<float, decltype (vr5), decltype (r172)> p1 { vr5, r172 };
    WDFSeriesT<float, decltype (inv1), decltype (p1)> s1 { inv1, p1 };

    WDFParallelT<float, decltype (s1), decltype (c45)> p2 { s1, c45 };
    WDFSeriesT<float, decltype (c47), decltype (vr4)> s3 { c47, vr4 };
    WDFSeriesT<float, decltype (s3), decltype (p2)> s2 { s3, p2 };

    WDFParallelT<float, decltype (vr4neg), decltype (s2)> p3 { vr4neg, s2 };
    WDFSeriesT<float, decltype (c49), decltype (r177)> s5 { c49, r177 };
    WDFSeriesT<float, decltype (s5), decltype (p3)> s4 { s5, p3 };

    PolarityInverterT<float, decltype (I1)> inv2 { I1 };
    WDFParallelT<float, decltype (s4), decltype (inv2)> p4 { s4, inv2 };

    YParameterT<float, decltype (p4)> y { p4, 0.261273e-3f, -2.61273e-4f, -7.86431e-2f, 78.6431e-3f };

    PolarityInverterT<float, decltype (r174)> inv3 { r174 };
    WDFSeriesT<float, decltype (inv3), decltype (c50)> s6 { inv3, c50 };
    WDFParallelT<float, decltype (s6), decltype (y)> p5 { s6, y };

    WDFParallelT<float, decltype (c48), decltype (p5)> p6 { c48, p5 };
    WDFParallelT<float, decltype (p6), decltype (VnegB2)> p7 { p6, VnegB2 };

    PolarityInverterT<float, decltype (p7)> inv4 { p7 };
    IdealCurrentSourceT<float, decltype (inv4)> I2 { inv4 };
};

#endif // TR808OUTPUTFILTER_H_INCLUDED
