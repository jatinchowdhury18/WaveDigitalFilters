#ifndef LEAF_H_INCLUDED
#define LEAF_H_INCLUDED

#include "Node.h"
#include "../IDs.h"

class Leaf : public Node
{
public:
    Leaf() {}
    virtual ~Leaf();

    bool getProbe() { return isProbe; }
    void setProbe (bool probe);

    void replaceNode (IDs::Leaf type);

private:
    bool isProbe = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Leaf)
};

#endif // LEAF_H_INCLUDED