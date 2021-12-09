#include "IdealIs.h"
#include "RootCells/IdealIsCell.h"

IdealIs::IdealIs() : current (new Property ({ "Current", 0.0f, -1.0f, 1.0f }))
{
    cell = std::make_unique<IdealIsCell> (*this);

    current->valueChanged = [=]
    {
        if (! getInput())
            setCurrent (current->value);
    };

    props.add (current);
}

IdealIs::~IdealIs()
{
    if (getInput())
        listeners.call (&Listener::changeInput, nullptr);
}

void IdealIs::setInput (bool input)
{
    Source::setInput (input);

    if (getInput())
        listeners.call (&Listener::changeInput, this);
    else
        setCurrent (current->value);
}

void IdealIs::setCurrent (float currentValue)
{
    if (idealIs != nullptr)
        idealIs->setCurrent (currentValue);
}

bool IdealIs::prepare (double sampleRate)
{
    if (child == nullptr)
        return false;

    if (! RootNode::prepare (sampleRate))
        return false;

    idealIs = std::make_unique<chowdsp::WDF::IdealCurrentSource<double>> (child->getWDF());
    wdf = idealIs.get();
    setCurrent (current->value);

    inputFunc = [=] (double x)
    {
        idealIs->setCurrent (x);
    };

    return true;
}
