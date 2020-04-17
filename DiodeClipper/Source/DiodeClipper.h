#ifndef DIODECLIPPER_H_INCLUDED
#define DIODECLIPPER_H_INCLUDED

#include "wdf.h"

using namespace WaveDigitalFilter;

template <typename T> inline int signum (T val)
{
    return (T (0) < val) - (val < T (0));
}

class DiodeClipper
{
public:
    DiodeClipper() {}

    void reset (double sampleRate, double cutoff)
    {
        double Cap = 47.0e-9;
        double Res = 4700.0;

        Vs.reset (new ResistiveVoltageSource);
        R1.reset (new Resistor (Res));
        C1.reset (new LeakyCapacitor (Cap, sampleRate));

        S1.reset (new  WDFSeries (Vs.get(), R1.get()));
        P1.reset (new WDFParallel (S1.get(), C1.get()));
        dp.connectToNode (P1.get());
        
        setCircuitParams (cutoff);
    }

    void setCircuitParams (double cutoff, double rPar = 100e6)
    {
        double Cap = 47.0e-9;
        double Res = 1.0 / (MathConstants<double>::twoPi * cutoff * Cap);

        C1->setCapacitanceValue (Cap);
        R1->setResistanceValue (Res);

        C1->setParallelResistance (rPar);
    }

    void setCircuitElements (float res, float cap, double rPar = 100e6)
    {
        C1->setCapacitanceValue ((double) cap);
        R1->setResistanceValue ((double) res);
        C1->setParallelResistance (rPar);
    }

    inline float processSample (float x)
    {
        Vs->setVoltage ((double) x);

        dp.incident (P1->reflected());
        y = C1->voltage();
        P1->incident (dp.reflected());

        return (float) y;
    }

private:
    std::unique_ptr<ResistiveVoltageSource> Vs;
    std::unique_ptr<Resistor> R1;
    std::unique_ptr<LeakyCapacitor> C1;
    
    std::unique_ptr<WDFSeries> S1;
    std::unique_ptr<WDFParallel> P1;
    double y = 0.0;

    // GZ34 diode pair
    DiodePair dp { 2.52e-9, 0.02585 };
};

#endif //DIODECLIPPER_H_INCLUDED
