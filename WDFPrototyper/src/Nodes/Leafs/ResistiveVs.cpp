#include "ResistiveVs.h"
#include "LeafCells/ResistiveVsCell.h"

ResistiveVs::ResistiveVs() : voltage (new Property ({ "Voltage", 0.0f, -20.0f, 20.0f })),
                             resistance (new Property ({ "Resistance", (float) 1.0e-9, (float) 1.0e-9, (float) 1.0e9 }))
{
    cell = std::make_unique<ResistiveVsCell> (*this);

    voltage->valueChanged = [=]
    {
        if (! getInput())
            setVoltage (voltage->value);
    };
    props.add (voltage);

    resistance->valueChanged = [=]
    {
        setResistance (resistance->value);
    };
    props.add (resistance);
}

ResistiveVs::~ResistiveVs()
{
    if (getInput())
        listeners.call (&Listener::changeInput, nullptr);
}

void ResistiveVs::setInput (bool input)
{
    Source::setInput (input);

    if (getInput())
        listeners.call (&Listener::changeInput, this);
    else
        setVoltage (voltage->value);
}

void ResistiveVs::setVoltage (float voltageValue)
{
    if (resVs != nullptr)
        resVs->setVoltage (voltageValue);
}

void ResistiveVs::setResistance (float resValue)
{
    if (resVs != nullptr)
        resVs->setResistanceValue (resValue);
}

bool ResistiveVs::prepare (double sampleRate)
{
    if (! Leaf::prepare (sampleRate))
        return false;

    resVs = std::make_unique<chowdsp::WDF::ResistiveVoltageSource<double>> (resistance->value);
    wdf = resVs.get();
    setVoltage (voltage->value);

    inputFunc = [=] (double x)
    {
        resVs->setVoltage (x);
    };

    return true;
}
