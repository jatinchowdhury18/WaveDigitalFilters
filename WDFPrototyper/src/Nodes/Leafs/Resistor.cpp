#include "Resistor.h"
#include "LeafCells/ResistorCell.h"

Resistor::Resistor() : resistor (new Property ({ "Resistance", 1000.0f, 1.0f, 1000000.0f }))
{
    cell = std::make_unique<ResistorCell> (*this);

    resistor->valueChanged = [=]
    {
        if (res != nullptr)
            res->setResistanceValue (resistor->value);
    };
    props.add (resistor);
}

bool Resistor::prepare (double sampleRate)
{
    if (! Leaf::prepare (sampleRate))
        return false;

    res = std::make_unique<chowdsp::wdf::Resistor<double>> (resistor->value);
    wdf = res.get();

    return true;
}
