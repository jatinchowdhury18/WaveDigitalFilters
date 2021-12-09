#include "RootNode.h"
#include "Diode.h"
#include "DiodePair.h"
#include "IdealIs.h"
#include "IdealVs.h"

void RootNode::setChild (IDs::Adaptor childType)
{
    listeners.call (&Listener::unprepare);

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

void RootNode::setChild (Node* newChild)
{
    child.reset (newChild);
    child->setParent (this);
}

void RootNode::replaceNode (IDs::Root type)
{
    std::unique_ptr<RootNode> newNode;

    if (type == IDs::Root::IdealVs)
        newNode = std::make_unique<IdealVs>();
    else if (type == IDs::Root::IdealIs)
        newNode = std::make_unique<IdealIs>();
    else if (type == IDs::Root::Diode)
        newNode = std::make_unique<Diode>();
    else if (type == IDs::Root::DiodePair)
        newNode = std::make_unique<DiodePair>();
    else
    {
        jassertfalse;
        return;
    }

    listeners.call (&Listener::replaceNode, this, newNode.release());
}

bool RootNode::prepare (double sampleRate)
{
    if (! Node::prepare (sampleRate))
        return false;

    if (child == nullptr)
        return false;

    return child->prepare (sampleRate);
}
