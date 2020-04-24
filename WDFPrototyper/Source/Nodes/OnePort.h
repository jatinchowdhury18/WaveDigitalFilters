#ifndef ONEPORT_H_INCLUDED
#define ONEPORT_H_INCLUDED

#include "Node.h"
#include "../IDs.h"

class OnePort : public Node
{
public:
    OnePort();
    virtual ~OnePort() {}

    Node* getChild() { return child.get(); }

    void setChild (IDs::Adaptor childType);
    void setChild (IDs::Leaf childType);
    void setChild (Node* newChild);

    bool prepare (double sampleRate) override;

protected:
    std::unique_ptr<Node> child;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePort)
};

#endif // ONEPORT_H_INCLUDED
