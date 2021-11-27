#include "RootNode.h"
#include "IdealVs.h"
#include "IdealIs.h"
#include "Diode.h"
#include "DiodePair.h"

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
    RootNode* newNode;

    if (type == IDs::Root::IdealVs)
        newNode = new IdealVs;
    else if (type == IDs::Root::IdealIs)
        newNode = new IdealIs;
    else if (type == IDs::Root::Diode)
        newNode = new Diode;
    else if (type == IDs::Root::DiodePair)
        newNode = new DiodePair;
    else
    {
        jassertfalse;
        return;
    }

    listeners.call (&Listener::replaceNode, this, newNode);
}

bool RootNode::prepare (double sampleRate)
{
    bool result = Node::prepare (sampleRate);

    if (child.get() != nullptr)
        result = child->prepare (sampleRate);
    else
        result = false;

    if (result)
        wdf.get()->connectToNode (child->getWDF());

    return result;
}
