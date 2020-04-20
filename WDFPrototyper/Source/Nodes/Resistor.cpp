#include "Resistor.h"
#include "../GUI/ResistorCell.h"

Resistor::Resistor()
{
    cell = std::make_unique<ResistorCell> (*this);
}

bool Resistor::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<WaveDigitalFilter::Resistor> (1000.0);

    return result;
}
