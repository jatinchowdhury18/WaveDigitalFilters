#ifndef LCOSCILLATOR_H_INCLUDED
#define LCOSCILLATOR_H_INCLUDED

#include "JuceHeader.h"
#include "wdf.h"

using namespace WaveDigitalFilter;

class LCOscillator
{
public:
    LCOscillator() {}

    void reset (double sampleRate, double freq, bool switchClosed)
    {
        C1.reset (new Capacitor (1.0e-3, sampleRate));
        I1.reset (new Inductor (1.0e3, sampleRate));

        Vs.reset (new ResistiveVoltageSource);
        Sw1.reset (new Switch);

        S2.reset (new WDFSeries (I1.get(), C1.get()));
        S1.reset (new WDFSeries (Vs.get(), S2.get()));
        S1->connectToNode (Sw1.get());

        setCircuitParams (freq, switchClosed);
    }

    void setCircuitParams (double freq, bool switchClosed)
    {
        const double cap = 1.0e-6;
        const double ind = 1.0 / (pow (MathConstants<double>::twoPi * freq, 2) * cap);

        C1->setCapacitanceValue (cap);
        I1->setInductanceValue (ind);
        Sw1->setClosed (switchClosed);
    }

    inline float processSample (float x)
    {
        Vs->setVoltage ((double) x);

        Sw1->incident (S1->reflected());
        S1->incident (Sw1->reflected());
        double y = C1->voltage();

        return (float) y;
    }

private:
    std::unique_ptr<ResistiveVoltageSource> Vs;
    std::unique_ptr<Switch> Sw1;
    std::unique_ptr<Capacitor> C1;
    std::unique_ptr<Inductor> I1;
    
    std::unique_ptr<WDFSeries> S1;
    std::unique_ptr<WDFSeries> S2;
};

#endif //LCOSCILLATOR_H_INCLUDED
