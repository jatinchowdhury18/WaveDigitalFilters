#ifndef ROOTCELL_H_INCLUDED
#define ROOTCELL_H_INCLUDED

#include "Cell.h"
#include "Roots/RootNode.h"
#include "../IDs.h"

class RootCell : public Cell
{
public:
    RootCell (RootNode& node) :
        Cell (node.getProps()),
        node (node)
    {}
    virtual ~RootCell() {}

    void getPopupMenu (PopupMenu& p) override
    {
        PopupMenu m;
        m.addItem ("Ideal Vs",   [=] { node.replaceNode (IDs::Root::IdealVs); });
        m.addItem ("Ideal Is",   [=] { node.replaceNode (IDs::Root::IdealIs); });
        m.addItem ("Diode",      [=] { node.replaceNode (IDs::Root::Diode); });
        m.addItem ("Diode Pair", [=] { node.replaceNode (IDs::Root::DiodePair); });

        p.addSubMenu ("Replace Node", m);

        PopupMenu childrenMenu;
        childrenMenu.addItem ("Series",   [=] { node.setChild (IDs::Adaptor::Series); });
        childrenMenu.addItem ("Parallel", [=] { node.setChild (IDs::Adaptor::Parallel); });
        childrenMenu.addItem ("Inverter", [=] { node.setChild (IDs::Adaptor::Inverter); });

        p.addSubMenu ("Set child", childrenMenu);

        Cell::getPopupMenu (p);
    }

private:
    RootNode& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RootCell)
};

#endif // ROOTCELL_H_INCLUDED

