#include "TR808OutputFilter.h"
#include <cmath> // pow()
 
void OutputFilter::reset (double sampleRate)
{
    inv1 = std::make_unique<PolarityInverter> (&Vbtr);
    p1 = std::make_unique<WDFParallel> (&vr5, &r172);
    s1 = std::make_unique<WDFSeries> (inv1.get(), p1.get());

    c45 = std::make_unique<Capacitor> (0.1e-6, sampleRate);
    p2 = std::make_unique<WDFParallel> (s1.get(), c45 .get());
    c47 = std::make_unique<Capacitor> (0.47e-6, sampleRate);
    s3 = std::make_unique<WDFSeries> (c47.get(), &vr4);
    s2 = std::make_unique<WDFSeries> (s3.get(), p2.get());

    p3 = std::make_unique<WDFParallel> (&vr4neg, s2.get());
    c49 = std::make_unique<Capacitor> (0.47e-6, sampleRate);
    s5 = std::make_unique<WDFSeries> (c49.get(), &r177);
    s4 = std::make_unique<WDFSeries> (s5.get(), p3.get());

    inv2 = std::make_unique<PolarityInverter> (&I1);
    p4 = std::make_unique<WDFParallel> (s4.get(), inv2.get());

    y = std::make_unique<YParameter> (p4.get(), 0.261273e-3, -2.61273e-4, -7.86431e-2, 78.6431e-3);

    inv3 = std::make_unique<PolarityInverter> (&r174);
    c50 = std::make_unique<Capacitor> (1.0e-6, sampleRate);
    s6 = std::make_unique<WDFSeries> (inv3.get(), c50.get());
    p5 = std::make_unique<WDFParallel> (s6.get(), y.get());

    c48 = std::make_unique<Capacitor> (220.0e-9, sampleRate);
    p6 = std::make_unique<WDFParallel> (c48.get(), p5.get());
    p7 = std::make_unique<WDFParallel> (p6.get(), &VnegB2);

    inv4 = std::make_unique<PolarityInverter> (p7.get());
    I2.connectToNode (inv4.get());

    I1.setCurrent (-0.169104e-3);
    I2.setCurrent (50.9002e-3);
    VnegB2.setVoltage (-15.0);
}

void OutputFilter::setVolume (float vol)
{
    // log taper potentiometer (rough approximation)
    double l = pow ((double) vol, 2);
    l = l < 0.001 ? 0.001 : (l > 0.999 ? 0.999 : l); // clamp

    vr4.setResistanceValue ((1.0 - l) * 100000.0);
    vr4neg.setResistanceValue (l * 100000.0);
}

void OutputFilter::setTone (float tone)
{
    // anti-log taper potentiometer (rough approximation)
    double c = pow ((double) tone, 0.5);
    c = c < 0.001 ? 0.001 : (c > 0.999 ? 0.999 : c); // clamp

    vr5.setResistanceValue ((1.0 - c) * 10000.0);
}
