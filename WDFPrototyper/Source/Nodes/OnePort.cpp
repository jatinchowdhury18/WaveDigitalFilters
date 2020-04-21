#include "OnePort.h"
#include "Series.h"
#include "Parallel.h"
#include "Inverter.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "ResistiveVs.h"
#include "ResistiveIs.h"

OnePort::OnePort()
{
}

void OnePort::setChild (IDs::Adaptor childType)
{
    if (childType == IDs::Adaptor::Series)
        child = std::make_unique<Series>();
    else if (childType == IDs::Adaptor::Parallel)
        child = std::make_unique<Parallel>();
    else if (childType == IDs::Adaptor::Inverter)
        child = std::make_unique<Inverter>();
    else
    {
        jassertfalse;
        return;
    }

    child->setParent (this);
    listeners.call (&Listener::addNode, this, child.get());
}

void OnePort::setChild (IDs::Leaf childType)
{
    if (childType == IDs::Leaf::Resistor)
        child = std::make_unique<Resistor>();
    else if (childType == IDs::Leaf::Capacitor)
        child = std::make_unique<Capacitor>();
    else if (childType == IDs::Leaf::Inductor)
        child = std::make_unique<Inductor>();
    else if (childType == IDs::Leaf::ResistiveVs)
        child = std::make_unique<ResistiveVs>();
    else if (childType == IDs::Leaf::ResistiveIs)
        child = std::make_unique<ResistiveIs>();
    else
    {
        jassertfalse;
        return;
    }

    child->setParent (this);
    listeners.call (&Listener::addNode, this, child.get());
}

void OnePort::setChild (Node* newChild)
{
    child.reset (newChild);
    child->setParent (this);
}

bool OnePort::prepare (double sampleRate)
{
    bool result = Node::prepare (sampleRate);

    if (child.get() != nullptr)
        result = child->prepare (sampleRate);
    else
        result = false;

    return result;
}
