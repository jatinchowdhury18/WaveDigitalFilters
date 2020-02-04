#ifndef DIODECLIPPER_H_INCLUDED
#define DIODECLIPPER_H_INCLUDED

#include "wdf.h"
#include "omega.h"

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
        C1.reset (new Capacitor (Cap, sampleRate));

        S1.reset (new  WDFSeries (Vs.get(), R1.get()));
        P1.reset (new WDFParallel (S1.get(), C1.get()));
        
        setCircuitParams (cutoff);
    }

    void setCircuitParams (double cutoff)
    {
        double Cap = 47.0e-9;
        double Res = 1.0 / (MathConstants<double>::twoPi * cutoff * Cap);

        C1->setCapacitanceValue (Cap);
        R1->setResistanceValue (Res);
    }

    void setCircuitElements (float res, float cap)
    {
        C1->setCapacitanceValue ((double) cap);
        R1->setResistanceValue ((double) res);
    }

    inline float processSample (float x)
    {
        Vs->setVoltage ((double) x);

        double a = P1->reflected();

        // Evaluate diode clipper nonlinearity in the wave domain
        // See Werner et al., "An Improved and Generalized Diode Clipper Model for Wave Digital Filters"
        // https://www.researchgate.net/publication/299514713_An_Improved_and_Generalized_Diode_Clipper_Model_for_Wave_Digital_Filters
        double lambda = (double) signum (a);
        double b = a + 2 * lambda * (P1->R * Is - Vt * omega4 (float (log (P1->R * Is / Vt) + (lambda * a + P1->R * Is) / Vt)));

        y = C1->voltage();

        P1->incident (b);

        return (float) y;
    }

private:
    std::unique_ptr<ResistiveVoltageSource> Vs;
    std::unique_ptr<Resistor> R1;
    std::unique_ptr<Capacitor> C1;
    
    std::unique_ptr<WDFSeries> S1;
    std::unique_ptr<WDFParallel> P1;
    double y = 0.0;

    // GZ34 diode
    const double Vt = 0.02585; // Thermal voltage
    const double Is = 2.52e-9; // reverse saturation current
};

#endif //DIODECLIPPER_H_INCLUDED
