#include "Inverter.h"
#include "AdaptorCells/InverterCell.h"

Inverter::Inverter()
{
    cell = std::make_unique<InverterCell> (*this);
}

bool Inverter::prepare (double sampleRate)
{
    bool result = OnePort::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<chowdsp::WDF::PolarityInverter<double>> (child->getWDF());

    return result;
}
