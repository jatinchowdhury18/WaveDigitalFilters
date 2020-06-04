#ifndef BAXANDALLTREBLE_H_INCLUDED
#define BAXANDALLTREBLE_H_INCLUDED

#include "JuceHeader.h"
#include "wdf.h"

using namespace WaveDigitalFilter;

class BaxandallTreble
{
public:
    BaxandallTreble() {}

    void reset (double sampleRate)
    {
        C3 = std::make_unique<Capacitor> (2.2e-9, sampleRate);
        C4 = std::make_unique<Capacitor> (22e-9, sampleRate);

        S1 = std::make_unique<WDFSeries> (&VR2b, C4.get());
        S2 = std::make_unique<WDFSeries> (S1.get(), &VR2a);
        S3 = std::make_unique<WDFSeries> (C3.get(), S2.get());

        Vin.connectToNode (S3.get());
    }

    void setTreble (float treble)
    {
        treble = jlimit (0.001f, 0.999f, treble);
        VR2a.setResistanceValue (100000.0 * (1.0 - (double) treble));
        VR2b.setResistanceValue (100000.0 * (double) treble);
    }

    inline float processSample (float x)
    {
        Vin.setVoltage ((double) x);

        Vin.incident (S3->reflected());
        S3->incident (Vin.reflected());
        auto y = C4->voltage() + VR2b.voltage();

        return (float) y;
    }

private:
    IdealVoltageSource Vin;
    Resistor VR2a { 100000.0 };
    Resistor VR2b { 100000.0 };

    std::unique_ptr<Capacitor> C3, C4;
    std::unique_ptr<WDFSeries> S1, S2, S3;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaxandallTreble)
};

#endif // BAXANDALLTREBLE_H_INCLUDED
