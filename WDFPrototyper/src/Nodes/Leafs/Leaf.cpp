#include "Leafs/Leaf.h"
#include "Leafs/Capacitor.h"
#include "Leafs/Inductor.h"
#include "Leafs/ResistiveIs.h"
#include "Leafs/ResistiveVs.h"
#include "Leafs/Resistor.h"

Leaf::~Leaf()
{
    if (isProbe)
        listeners.call (&Listener::changeProbe, nullptr);
}

void Leaf::setProbe (bool probe)
{
    isProbe = probe;

    if (probe)
        listeners.call (&Listener::changeProbe, this);
}

void Leaf::replaceNode (IDs::Leaf type)
{
    std::unique_ptr<Node> newNode;

    if (type == IDs::Leaf::Resistor)
        newNode = std::make_unique<Resistor>();
    else if (type == IDs::Leaf::Capacitor)
        newNode = std::make_unique<Capacitor>();
    else if (type == IDs::Leaf::Inductor)
        newNode = std::make_unique<Inductor>();
    else if (type == IDs::Leaf::ResistiveVs)
        newNode = std::make_unique<ResistiveVs>();
    else if (type == IDs::Leaf::ResistiveIs)
        newNode = std::make_unique<ResistiveIs>();
    else
    {
        jassertfalse;
        return;
    }

    listeners.call (&Listener::replaceNode, this, newNode.release());
}
