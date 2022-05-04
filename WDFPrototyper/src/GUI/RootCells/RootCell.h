#ifndef ROOTCELL_H_INCLUDED
#define ROOTCELL_H_INCLUDED

#include "../IDs.h"
#include "Cell.h"
#include "Roots/RootNode.h"

class RootCell : public Cell
{
public:
    explicit RootCell (RootNode& node) : Cell (node.getProps()),
                                         node (node)
    {
    }
    ~RootCell() override = default;

    void getPopupMenu (juce::PopupMenu& p) override
    {
        juce::PopupMenu m;
        m.addItem ("Ideal Vs", [=]
                   { node.replaceNode (IDs::Root::IdealVs); });
        m.addItem ("Ideal Is", [=]
                   { node.replaceNode (IDs::Root::IdealIs); });
        m.addItem ("Diode", [=]
                   { node.replaceNode (IDs::Root::Diode); });
        m.addItem ("Diode Pair", [=]
                   { node.replaceNode (IDs::Root::DiodePair); });

        p.addSubMenu ("Replace Node", m);

        juce::PopupMenu childrenMenu;
        childrenMenu.addItem ("Series", [=]
                              { node.setChild (IDs::Adaptor::Series); });
        childrenMenu.addItem ("Parallel", [=]
                              { node.setChild (IDs::Adaptor::Parallel); });
        childrenMenu.addItem ("Inverter", [=]
                              { node.setChild (IDs::Adaptor::Inverter); });

        p.addSubMenu ("Set child", childrenMenu);

        Cell::getPopupMenu (p);
    }

private:
    RootNode& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RootCell)
};

#endif // ROOTCELL_H_INCLUDED
