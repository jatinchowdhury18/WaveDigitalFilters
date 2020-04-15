#ifndef TR808OUTPUTFILTER_H_INCLUDED
#define TR808OUTPUTFILTER_H_INCLUDED

#include "wdf.h"
#include <memory>

using namespace WaveDigitalFilter;

class OutputFilter
{
public:
    OutputFilter() {}

    void reset (double sampleRate);
    void setVolume (float vol);
    void setTone (float tone);

    inline float processSample (float x)
    {
        Vbtr.setVoltage ((double) x);

        I2.incident (inv4->reflected());
        double voltageOut = r174.voltage();
        inv4->incident (I2.reflected());

        return (float) voltageOut;
    }

private:
    Resistor r172 {  10000.0 };
    Resistor r174 { 100000.0 };
    Resistor r177 {  82000.0 };

    // potentiometers
    Resistor vr4 { 100000.0 }; // (1-l)VR_4, VR_4 goes up to 100k
    Resistor vr4neg { 0.1 };   // other side of vr4
    Resistor vr5 { 10000.0 };  // (1-c)VR_5, VR_5 goes up to 10k

    ResistiveVoltageSource Vbtr { 220.0 };
    ResistiveCurrentSource I1 { 100000.0 };
    ResistiveVoltageSource VnegB2 { 6800.0 };
    IdealCurrentSource I2; // root

    std::unique_ptr<Capacitor> c45;
    std::unique_ptr<Capacitor> c47;
    std::unique_ptr<Capacitor> c48;
    std::unique_ptr<Capacitor> c49;
    std::unique_ptr<Capacitor> c50;

    std::unique_ptr<YParameter> y;

    std::unique_ptr<PolarityInverter> inv1;
    std::unique_ptr<PolarityInverter> inv2;
    std::unique_ptr<PolarityInverter> inv3;
    std::unique_ptr<PolarityInverter> inv4;

    std::unique_ptr<WDFSeries> s1;
    std::unique_ptr<WDFSeries> s2;
    std::unique_ptr<WDFSeries> s3;
    std::unique_ptr<WDFSeries> s4;
    std::unique_ptr<WDFSeries> s5;
    std::unique_ptr<WDFSeries> s6;

    std::unique_ptr<WDFParallel> p1;
    std::unique_ptr<WDFParallel> p2;
    std::unique_ptr<WDFParallel> p3;
    std::unique_ptr<WDFParallel> p4;
    std::unique_ptr<WDFParallel> p5;
    std::unique_ptr<WDFParallel> p6;
    std::unique_ptr<WDFParallel> p7;
};

#endif // TR808OUTPUTFILTER_H_INCLUDED
