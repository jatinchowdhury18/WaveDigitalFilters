#include "TwoPort.h"
#include "Resistor.h"
#include "Series.h"

void TwoPort::setChild (int idx, IDs::Adaptor childType)
{
    if (childType == IDs::Adaptor::Series)
        child[idx] = std::make_unique<Series>();
    else
    {
        jassertfalse;
        return;
    }

    child[idx]->setParent (this);
    listeners.call (&Listener::addNode ,this, child[idx].get());
}

void TwoPort::setChild (int idx, IDs::Leaf childType)
{
    if (childType == IDs::Leaf::Resistor)
        child[idx] = std::make_unique<Resistor>();
    else
    {
        jassertfalse;
        return;
    }

    child[idx]->setParent (this);
    listeners.call (&Listener::addNode ,this, child[idx].get());
    // connect to node
}

bool TwoPort::prepare (double sampleRate)
{
    bool result = Node::prepare (sampleRate);

    for (int i = 0; i < 2; ++i)
    {
        if (child[i].get() != nullptr)
            result = child[i]->prepare (sampleRate);
        else
            result = false;
    }

    return result;
}
