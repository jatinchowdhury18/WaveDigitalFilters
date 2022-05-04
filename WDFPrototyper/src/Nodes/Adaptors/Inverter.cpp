#include "Inverter.h"
#include "AdaptorCells/InverterCell.h"

Inverter::Inverter()
{
    cell = std::make_unique<InverterCell> (*this);
}

bool Inverter::prepare (double sampleRate)
{
    if (! OnePort::prepare (sampleRate))
        return false;

    inverter = std::make_unique<chowdsp::wdf::PolarityInverter<double>> (child->getWDF());
    wdf = inverter.get();

    return true;
}
