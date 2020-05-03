#include "IdealIs.h"
#include "RootCells/IdealIsCell.h"

IdealIs::IdealIs() :
    current (new Property ({"Current", 0.0f, -1.0f, 1.0f}))
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
    if (auto is = dynamic_cast<WaveDigitalFilter::IdealCurrentSource*> (wdf.get()))
        is->setCurrent (currentValue);
}

bool IdealIs::prepare (double sampleRate)
{
    wdf = std::make_unique<WaveDigitalFilter::IdealCurrentSource>();
    setCurrent (current->value);

    bool result = RootNode::prepare (sampleRate);

    if (result)
    {
        inputFunc = [=] (double x)
        {
            dynamic_cast<WaveDigitalFilter::IdealCurrentSource*> (wdf.get())->setCurrent (x);
        };
    }

    return result;
}
