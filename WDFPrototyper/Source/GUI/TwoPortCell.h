#ifndef TWOPORTCELL_H_INCLUDED
#define TWOPORTCELL_H_INCLUDED

#include "Cell.h"
#include "../Nodes/TwoPort.h"

class TwoPortCell : public Cell
{
public:
    TwoPortCell (TwoPort& node) :
        node (node)
    {}
    virtual ~TwoPortCell() {}

    void getPopupMenu (PopupMenu& p) override
    {
        auto addChildren = [=] (PopupMenu& m, int idx)
        {
            m.addItem ("Series",   [=] { node.setChild (idx, IDs::Adaptor::Series); });
            m.addItem ("Parallel", {});
            m.addItem ("Inverter", {});
            m.addItem ("Resistor", [=] { node.setChild (idx, IDs::Leaf::Resistor); });
            m.addItem ("Capacitor", {});
            m.addItem ("Inductor", {});
        };

        PopupMenu childrenMenu1;
        addChildren (childrenMenu1, 0);

        PopupMenu childrenMenu2;
        addChildren (childrenMenu2, 1);

        p.addSubMenu ("Set child 1", childrenMenu1);
        p.addSubMenu ("Set child 2", childrenMenu2);
    }

private:
    TwoPort& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoPortCell)
};

#endif // TWOPORTCELL_H_INCLUDED
