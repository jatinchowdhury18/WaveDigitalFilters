#ifndef PASSIVELPF_H_INCLUDED
#define PASSIVELPF_H_INCLUDED

#include "JuceHeader.h"
#include "wdf.h"

using namespace WaveDigitalFilter;

class PassiveLPF
{
public:
    PassiveLPF()
    {
    }

    void reset (double sampleRate, double cutoff)
    {
        R1.reset (new Resistor (1000.0));
        R2.reset (new Resistor (1000.0));

        C1.reset (new Capacitor (5.0e-8, sampleRate));
        C2.reset (new Capacitor (5.0e-8, sampleRate));

        Vs.reset(new IdealVoltageSource());

        S1.reset (new WDFSeries (R2.get(), C2.get()));
        P1.reset (new WDFParallel (C1.get(), S1.get()));
        S2.reset (new WDFSeries (R1.get(), P1.get()));
        S2->connectToNode (Vs.get());

        setCutoffFrequency (cutoff);
    }

    void setCutoffFrequency (double cutoff)
    {
        const double Cap = 5.0e-8;
        const double Res = 1.0 / (MathConstants<double>::twoPi * cutoff * Cap);
        
        R1->setResistanceValue (Res);
        R2->setResistanceValue (Res);
        C1->setCapacitanceValue (Cap);
        C2->setCapacitanceValue (Cap);
    }

    inline float processSample (float x)
    {
        Vs->setVoltage ((double) x);

        Vs->incident (S2->reflected());
        double y = C2->voltage();
        S2->incident (Vs->reflected());

        return (float) y;
    }

private:
    std::unique_ptr<Resistor> R1;
    std::unique_ptr<Resistor> R2;
    std::unique_ptr<Capacitor> C1; // root
    std::unique_ptr<Capacitor> C2;
    std::unique_ptr<IdealVoltageSource> Vs;

    std::unique_ptr<WDFSeries> S1;
    std::unique_ptr<WDFSeries> S2;
    std::unique_ptr<WDFParallel> P1;
    std::unique_ptr<WDFParallel> P2;

    float cutoff;
};

#endif //PASSIVELPF_H_INCLUDED
