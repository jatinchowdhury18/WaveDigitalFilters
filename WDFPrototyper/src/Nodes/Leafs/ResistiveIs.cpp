#include "ResistiveIs.h"
#include "LeafCells/ResistiveIsCell.h"

ResistiveIs::ResistiveIs() :
    current (new Property ({"Current", 0.0f, -1.0f, 1.0f})),
    resistance (new Property ({"Resistance", (float) 1.0e9, (float) 1.0e-9, (float) 1.0e-9}))
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
        if (! getInput())
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
    if (auto is = dynamic_cast<chowdsp::WDF::ResistiveCurrentSource<double>*> (wdf.get()))
        is->setCurrent (currentValue);
}

void ResistiveIs::setResistance (float resValue)
{
    if (auto is = dynamic_cast<chowdsp::WDF::ResistiveCurrentSource<double>*> (wdf.get()))
        is->setResistanceValue (resValue);
}

bool ResistiveIs::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
    {
        wdf = std::make_unique<chowdsp::WDF::ResistiveCurrentSource<double>> (resistance->value);
        setCurrent (current->value);

        inputFunc = [=] (double x)
        {
            dynamic_cast<chowdsp::WDF::ResistiveCurrentSource<double>*> (wdf.get())->setCurrent (x);
        };
    }

    return result;
}
