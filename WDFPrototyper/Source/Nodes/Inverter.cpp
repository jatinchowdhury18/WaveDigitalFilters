#include "Inverter.h"
#include "../GUI/InverterCell.h"

Inverter::Inverter()
{
    cell = std::make_unique<InverterCell> (*this);
}

bool Inverter::prepare (double sampleRate)
{
    bool result = OnePort::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<WaveDigitalFilter::PolarityInverter> (child->getWDF());

    return result;
}
