#ifndef ROOTCELL_H_INCLUDED
#define ROOTCELL_H_INCLUDED

#include "Cell.h"
#include "../Nodes/RootNode.h"
#include "../IDs.h"

class RootCell : public Cell
{
public:
    RootCell (RootNode& node) :
        node (node)
    {}
    virtual ~RootCell() {}

    void getPopupMenu (PopupMenu& p) override
    {
        PopupMenu childrenMenu;
        childrenMenu.addItem ("Series",   [=] { node.setChild (IDs::Adaptor::Series); });
        childrenMenu.addItem ("Parallel", [=] { node.setChild (IDs::Adaptor::Parallel); });
        childrenMenu.addItem ("Inverter", [=] { node.setChild (IDs::Adaptor::Inverter); });

        p.addSubMenu ("Set child", childrenMenu);
    }

private:
    RootNode& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RootCell)
};

#endif // ROOTCELL_H_INCLUDED

