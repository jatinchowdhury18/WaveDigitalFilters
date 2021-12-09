#include "Leafs/Leaf.h"
#include "Leafs/Resistor.h"
#include "Leafs/Capacitor.h"
#include "Leafs/Inductor.h"
#include "Leafs/ResistiveIs.h"
#include "Leafs/ResistiveVs.h"

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
        newNode = std::unique_ptr<Resistor>();
    else if (type == IDs::Leaf::Capacitor)
        newNode = std::unique_ptr<Capacitor>();
    else if (type == IDs::Leaf::Inductor)
        newNode = std::unique_ptr<Inductor>();
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
