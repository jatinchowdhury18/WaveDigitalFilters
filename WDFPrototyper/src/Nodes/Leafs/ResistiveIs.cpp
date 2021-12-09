#include "ResistiveIs.h"
#include "LeafCells/ResistiveIsCell.h"

ResistiveIs::ResistiveIs() : current (new Property ({ "Current", 0.0f, -1.0f, 1.0f })),
                             resistance (new Property ({ "Resistance", (float) 1.0e9, (float) 1.0e-9, (float) 1.0e9 }))
{
    cell = std::make_unique<ResistiveIsCell> (*this);

    current->valueChanged = [=]
    {
        if (! getInput())
            setCurrent (current->value);
    };
    props.add (current);

    resistance->valueChanged = [=]
    {
        setResistance (resistance->value);
    };
    props.add (resistance);
}

ResistiveIs::~ResistiveIs()
{
    if (getInput())
        listeners.call (&Listener::changeInput, nullptr);
}

void ResistiveIs::setInput (bool input)
{
    Source::setInput (input);

    if (getInput())
        listeners.call (&Listener::changeInput, this);
    else
        setCurrent (current->value);
}

void ResistiveIs::setCurrent (float currentValue)
{
    if (resIs != nullptr)
        resIs->setCurrent (currentValue);
}

void ResistiveIs::setResistance (float resValue)
{
    if (resIs != nullptr)
        resIs->setResistanceValue (resValue);
}

bool ResistiveIs::prepare (double sampleRate)
{
    if (! Leaf::prepare (sampleRate))
        return false;

    resIs = std::make_unique<chowdsp::WDF::ResistiveCurrentSource<double>> (resistance->value);
    wdf = resIs.get();
    setCurrent (current->value);

    inputFunc = [=] (double x)
    {
        resIs->setCurrent (x);
    };

    return true;
}
