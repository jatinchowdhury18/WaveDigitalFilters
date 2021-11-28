#include "Capacitor.h"
#include "LeafCells/CapacitorCell.h"

Capacitor::Capacitor() :
    capacitance (new Property ({"Capacitance", (float) 1.0e-6, (float) 1.0e-12, 0.01f}))
{
    cell = std::make_unique<CapacitorCell> (*this);

    capacitance->valueChanged = [=]
    {
        if (auto cap = dynamic_cast<chowdsp::WDF::Capacitor<double>*> (wdf.get()))
            cap->setCapacitanceValue (capacitance->value);
    };
    props.add (capacitance);
}

bool Capacitor::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<chowdsp::WDF::Capacitor<double>> (capacitance->value, sampleRate);

    return result;
}
