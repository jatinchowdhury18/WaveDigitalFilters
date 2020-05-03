#include "TwoPort.h"
#include "Series.h"
#include "Parallel.h"
#include "Inverter.h"
#include "Leafs/Resistor.h"
#include "Leafs/Capacitor.h"
#include "Leafs/Inductor.h"
#include "Leafs/ResistiveVs.h"
#include "Leafs/ResistiveIs.h"

void TwoPort::setChild (int idx, IDs::Adaptor childType)
{
    listeners.call (&Listener::unprepare);

    if (childType == IDs::Adaptor::Series)
        child[idx] = std::make_unique<Series>();
    else if (childType == IDs::Adaptor::Parallel)
        child[idx] = std::make_unique<Parallel>();
    else if (childType == IDs::Adaptor::Inverter)
        child[idx] = std::make_unique<Inverter>();
    else
    {
        jassertfalse;
        return;
    }

    child[idx]->setParent (this);
    listeners.call (&Listener::addNode, this, child[idx].get());
}

void TwoPort::setChild (int idx, IDs::Leaf childType)
{
    listeners.call (&Listener::unprepare);

    if (childType == IDs::Leaf::Resistor)
        child[idx] = std::make_unique<Resistor>();
    else if (childType == IDs::Leaf::Capacitor)
        child[idx] = std::make_unique<Capacitor>();
    else if (childType == IDs::Leaf::Inductor)
        child[idx] = std::make_unique<Inductor>();
    else if (childType == IDs::Leaf::ResistiveVs)
        child[idx] = std::make_unique<ResistiveVs>();
    else if (childType == IDs::Leaf::ResistiveIs)
        child[idx] = std::make_unique<ResistiveIs>();
    else
    {
        jassertfalse;
        return;
    }

    child[idx]->setParent (this);
    listeners.call (&Listener::addNode, this, child[idx].get());
}

void TwoPort::setChild (int idx, Node* newChild)
{
    child[idx].reset (newChild);
    child[idx]->setParent (this);
}

void TwoPort::replaceNode (IDs::Adaptor type)
{
    Node* newNode;

    if (type == IDs::Adaptor::Series)
        newNode = new Series;
    else if (type == IDs::Adaptor::Parallel)
        newNode = new Parallel;
    else
    {
        jassertfalse;
        return;
    }

    listeners.call (&Listener::replaceNode, this, newNode);
}

bool TwoPort::prepare (double sampleRate)
{
    bool result = Node::prepare (sampleRate);

    for (int i = 0; (i < 2) && result; ++i)
    {
        if (child[i].get() != nullptr)
            result = child[i]->prepare (sampleRate);
        else
            result = false;
    }

    return result;
}
