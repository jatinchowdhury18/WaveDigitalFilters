#include "IdealVs.h"
#include "RootCells/IdealVsCell.h"

IdealVs::IdealVs() :
    voltage (new Property ({"Voltage", 0.0f, -20.0f, 20.0f}))
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
    if (auto vs = dynamic_cast<chowdsp::WDF::IdealVoltageSource<double>*> (wdf.get()))
        vs->setVoltage (voltageValue);
}

bool IdealVs::prepare (double sampleRate)
{
    if (child == nullptr || child->getWDF() == nullptr)
        return false;

    wdf = std::make_unique<chowdsp::WDF::IdealVoltageSource<double>> (child->getWDF());
    setVoltage (voltage->value);

    bool result = RootNode::prepare (sampleRate);

    if (result)
    {
        inputFunc = [=] (double x)
        {
            dynamic_cast<chowdsp::WDF::IdealVoltageSource<double>*> (wdf.get())->setVoltage (x);
        };
    }

    return result;
}

void IdealVs::childUpdated()
{
    wdf = std::make_unique<chowdsp::WDF::IdealVoltageSource<double>> (child->getWDF());
    setVoltage (voltage->value);
}
