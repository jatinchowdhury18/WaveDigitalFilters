#ifndef ROOTNODE_H_INCLUDED
#define ROOTNODE_H_INCLUDED

#include "Node.h"
#include "../IDs.h"
#include "Series.h"
#include "Parallel.h"
#include "Inverter.h"

class RootNode : public Node
{
public:
    RootNode() {}
    virtual ~RootNode() {}

    Node* getChild() { return child.get(); }

    void setChild (IDs::Adaptor childType)
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

    bool prepare (double sampleRate)
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

private:
    std::unique_ptr<Node> child;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RootNode)
};

#endif // ROOTNODE_H_INCLUDED
