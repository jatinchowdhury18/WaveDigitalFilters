#include "Capacitor.h"
#include "../GUI/CapacitorCell.h"

Capacitor::Capacitor() :
    capacitance (new Property ({"Capacitance", (float) 1.0e-6, (float) 1.0e-12, 0.01f}))
{
    cell = std::make_unique<CapacitorCell> (*this);

    capacitance->valueChanged = [=]
    {
        if (auto cap = dynamic_cast<WaveDigitalFilter::Capacitor*> (wdf.get()))
            cap->setCapacitanceValue (capacitance->value);
    };
    props.add (capacitance);
}

bool Capacitor::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<WaveDigitalFilter::Capacitor> (capacitance->value, sampleRate);

    return result;
}
