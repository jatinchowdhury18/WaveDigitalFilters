#ifndef PULSESHAPER_H_INCLUDED
#define PULSESHAPER_H_INCLUDED

#include "wdf.h"
#include <memory>

using namespace WaveDigitalFilter;

class PulseShaper
{
public:
    PulseShaper() {}

    void reset (double sampleRate)
    {
        c40 = std::make_unique<Capacitor> (0.015e-6, sampleRate, 0.029);
        P1 = std::make_unique<WDFParallel> (c40.get(), &r163);
        S1 = std::make_unique<WDFSeries> (&Vs, P1.get());

        I1 = std::make_unique<PolarityInverter> (&r162);
        P2 = std::make_unique<WDFParallel> (I1.get(), S1.get());

        d53.connectToNode (P2.get());
    }

    inline float processSample (float x)
    {
        Vs.setVoltage (x);

        d53.incident (P2->reflected());
        double y = r162.voltage();
        P2->incident (d53.reflected());

        return (float) y;
    }

private:
    ResistiveVoltageSource Vs;

    Resistor r162 {   4700.0 };
    Resistor r163 { 100000.0 };
    std::unique_ptr<Capacitor> c40;
    Diode d53 { 2.52e-9, 25.85e-3 }; // 1N4148 diode
    
    std::unique_ptr<PolarityInverter> I1;
    std::unique_ptr<WDFSeries> S1;
    std::unique_ptr<WDFParallel> P1;
    std::unique_ptr<WDFParallel> P2;
};

#endif // PULSESHAPER_H_INCLUDED
