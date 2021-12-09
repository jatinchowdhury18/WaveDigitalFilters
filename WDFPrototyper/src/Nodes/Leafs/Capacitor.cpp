#include "Capacitor.h"
#include "LeafCells/CapacitorCell.h"

Capacitor::Capacitor() :
    capacitance (new Property ({"Capacitance", (float) 1.0e-6, (float) 1.0e-12, 0.01f}))
{
    cell = std::make_unique<CapacitorCell> (*this);

    capacitance->valueChanged = [=]
    {
        if (cap != nullptr)
            cap->setCapacitanceValue (capacitance->value);
    };
    props.add (capacitance);
}

bool Capacitor::prepare (double sampleRate)
{
    if (! Leaf::prepare (sampleRate))
        return false;

    cap = std::make_unique<chowdsp::WDF::Capacitor<double>> (capacitance->value, sampleRate);
    wdf = cap.get();

    return true;
}
