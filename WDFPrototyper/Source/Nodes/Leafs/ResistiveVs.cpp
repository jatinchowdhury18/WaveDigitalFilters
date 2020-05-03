#include "ResistiveVs.h"
#include "LeafCells/ResistiveVsCell.h"

ResistiveVs::ResistiveVs() :
    voltage (new Property ({"Voltage", 0.0f, -20.0f, 20.0f})),
    resistance (new Property ({"Resistance", (float) 1.0e-9, (float) 1.0e-9, (float) 1.0e-9}))
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
        if (! getInput())
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
    if (auto vs = dynamic_cast<WaveDigitalFilter::ResistiveVoltageSource*> (wdf.get()))
        vs->setVoltage (voltageValue);
}

void ResistiveVs::setResistance (float resValue)
{
    if (auto vs = dynamic_cast<WaveDigitalFilter::ResistiveVoltageSource*> (wdf.get()))
        vs->setResistanceValue (resValue);
}

bool ResistiveVs::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
    {
        wdf = std::make_unique<WaveDigitalFilter::ResistiveVoltageSource> (resistance->value);
        setVoltage (voltage->value);

        inputFunc = [=] (double x)
        {
            dynamic_cast<WaveDigitalFilter::ResistiveVoltageSource*> (wdf.get())->setVoltage (x);
        };
    }

    return result;
}
