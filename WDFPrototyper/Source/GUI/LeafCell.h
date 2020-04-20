#ifndef LEAFCELL_H_INCLUDED
#define LEAFCELL_H_INCLUDED

#include "Cell.h"
#include "../Nodes/Leaf.h"

class LeafCell : public Cell
{
public:
    LeafCell (Leaf& node) :
        node (node)
    {}
    virtual ~LeafCell() {}

    void getPopupMenu (PopupMenu& p) override
    {
        p.addItem ("Probe", true, node.getProbe(), [=] { node.setProbe (! node.getProbe()); });
    }

private:
    Leaf& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeafCell)
};

#endif // LEAFCELL_H_INCLUDED