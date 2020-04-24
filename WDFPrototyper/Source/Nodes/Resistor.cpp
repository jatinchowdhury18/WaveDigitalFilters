#include "Resistor.h"
#include "../GUI/ResistorCell.h"

Resistor::Resistor() :
    resistor (new Property ({"Resistance", 1000.0f, 1.0f, 1000000.0f}))
{
    cell = std::make_unique<ResistorCell> (*this);

    resistor->valueChanged = [=]
    {
        if (auto res = dynamic_cast<WaveDigitalFilter::Resistor*> (wdf.get()))
            res->setResistanceValue (resistor->value);
    };
    props.add (resistor);
}

bool Resistor::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<WaveDigitalFilter::Resistor> (resistor->value);

    return result;
}
