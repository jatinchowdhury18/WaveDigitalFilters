#ifndef TWOPORT_H_INCLUDED
#define TWOPORT_H_INCLUDED

#include "Node.h"
#include "../IDs.h"

class TwoPort : public Node
{
public:
    TwoPort() {}
    virtual ~TwoPort() {}

    Node* getChild (int idx) { return child[idx].get(); }

    void setChild (int idx, IDs::Adaptor childType);
    void setChild (int idx, IDs::Leaf childType);

    bool prepare (double sampleRate);

protected:
    std::unique_ptr<Node> child[2];

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoPort)
};

#endif // TWOPORT_H_INCLUDED
