#include "IdealVs.h"
#include "RootCells/IdealVsCell.h"

IdealVs::IdealVs() : voltage (new Property ({ "Voltage", 0.0f, -20.0f, 20.0f }))
{
    cell = std::make_unique<IdealVsCell> (*this);

    voltage->valueChanged = [=]
    {
        if (! getInput())
            setVoltage (voltage->value);
    };

    props.add (voltage);
}

IdealVs::~IdealVs()
{
    if (getInput())
        listeners.call (&Listener::changeInput, nullptr);
}

void IdealVs::setInput (bool input)
{
    Source::setInput (input);

    if (getInput())
        listeners.call (&Listener::changeInput, this);
    else
        setVoltage (voltage->value);
}

void IdealVs::setVoltage (float voltageValue)
{
    if (idealVs != nullptr)
        idealVs->setVoltage (voltageValue);
}

bool IdealVs::prepare (double sampleRate)
{
    if (child == nullptr)
        return false;

    if (! RootNode::prepare (sampleRate))
        return false;

    idealVs = std::make_unique<chowdsp::WDF::IdealVoltageSource<double>> (child->getWDF());
    wdf = idealVs.get();
    setVoltage (voltage->value);

    inputFunc = [=] (double x)
    {
        idealVs->setVoltage (x);
    };

    return true;
}
