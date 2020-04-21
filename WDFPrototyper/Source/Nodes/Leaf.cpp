#include "Leaf.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Inductor.h"

void Leaf::setProbe (bool probe)
{ 
    isProbe = probe;

    if (probe)
        listeners.call (&Listener::changeProbe, this);
}

void Leaf::replaceNode (IDs::Leaf type)
{
    Node* newNode;
    
    if (type == IDs::Leaf::Resistor)
        newNode = new Resistor;
    else if (type == IDs::Leaf::Capacitor)
        newNode = new Capacitor;
    else if (type == IDs::Leaf::Inductor)
        newNode = new Inductor;
    else
    {
        jassertfalse;
        return;
    }

    listeners.call (&Listener::replaceNode, this, newNode);
}
