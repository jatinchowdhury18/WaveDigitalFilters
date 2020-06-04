#ifndef BAXANDALLBASS_H_INCLUDED
#define BAXANDALLBASS_H_INCLUDED

#include "JuceHeader.h"
#include "wdf.h"

using namespace WaveDigitalFilter;

class BaxandallBass
{
public:
    BaxandallBass() {}

    void reset (double sampleRate)
    {
        C1 = std::make_unique<Capacitor> (22e-9, sampleRate);
        C2 = std::make_unique<Capacitor> (220e-9, sampleRate);
        C4 = std::make_unique<Capacitor> (22e-9, sampleRate);

        P1 = std::make_unique<WDFParallel> (C1.get(), &VR1a);
        P2 = std::make_unique<WDFParallel> (C2.get(), &VR1b);

        S1 = std::make_unique<WDFSeries> (C4.get(), &VR2b);
        S2 = std::make_unique<WDFSeries> (S1.get(), &R3);

        S3 = std::make_unique<WDFSeries> (P1.get(), &R2);
        P3 = std::make_unique<WDFParallel> (S3.get(), S2.get());
        S4 = std::make_unique<WDFSeries> (P3.get(), P2.get());
        S5 = std::make_unique<WDFSeries> (S4.get(), &R1);

        Vin.connectToNode (S5.get());
    }

    void setBass (float bass)
    {
        bass = jlimit (0.001f, 0.999f, bass);
        VR1b.setResistanceValue (100000.0 * (1.0 - (double) bass));
        VR1a.setResistanceValue (100000.0 * (double) bass);
    }

    void setTreble (float treble)
    {
        treble = jlimit (0.001f, 0.999f, treble);
        VR2b.setResistanceValue (100000.0 * (double) treble);
    }

    inline float processSample (float x) noexcept
    {
        Vin.setVoltage ((double) x);

        Vin.incident (S5->reflected());
        S5->incident (Vin.reflected());
        auto y = C4->voltage() + VR2b.voltage();

        return (float) y;
    }

private:
    IdealVoltageSource Vin;
    Resistor R1 { 10000.0 };
    Resistor R2 { 1000.0 };
    Resistor R3 { 10000.0 };

    Resistor VR1a { 100000.0 };
    Resistor VR1b { 100000.0 };
    Resistor VR2b { 100000.0 };

    std::unique_ptr<Capacitor> C1, C2, C4;
    std::unique_ptr<WDFSeries> S1, S2, S3, S4, S5;
    std::unique_ptr<WDFParallel> P1, P2, P3;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaxandallBass)
};

#endif // BAXANDALLBASS_H_INCLUDED
