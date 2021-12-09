#ifndef ROOTNODE_H_INCLUDED
#define ROOTNODE_H_INCLUDED

#include "Node.h"
#include "../IDs.h"
#include "Adaptors/Series.h"
#include "Adaptors/Parallel.h"
#include "Adaptors/Inverter.h"

class RootNode : public Node
{
public:
    RootNode() = default;
    ~RootNode() override = default;

    Node* getChild() { return child.get(); }
    Node* getChildReleased() { return child.release(); }

    void setChild (IDs::Adaptor childType);
    void setChild (Node* newChild);

    void replaceNode (IDs::Root type);

    bool prepare (double sampleRate) override;

protected:
    std::unique_ptr<Node> child;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RootNode)
};

#endif // ROOTNODE_H_INCLUDED
