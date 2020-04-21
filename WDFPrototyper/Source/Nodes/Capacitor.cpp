#include "Capacitor.h"
#include "../GUI/CapacitorCell.h"

Capacitor::Capacitor()
{
    cell = std::make_unique<CapacitorCell> (*this);
}

bool Capacitor::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<WaveDigitalFilter::Capacitor> (1.0e-6, sampleRate);

    return result;
}
