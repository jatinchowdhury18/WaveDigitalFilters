#ifndef ONEPORTCELL_H_INCLUDED
#define ONEPORTCELL_H_INCLUDED

#include "Cell.h"
#include "../Nodes/OnePort.h"

class OnePortCell : public Cell
{
public:
    OnePortCell (OnePort& node) :
        node (node)
    {}
    virtual ~OnePortCell() {}

    void getPopupMenu (PopupMenu& p) override
    {
        PopupMenu m;

        m.addItem ("Series",    [=] { node.setChild (IDs::Adaptor::Series); });
        m.addItem ("Parallel",  [=] { node.setChild (IDs::Adaptor::Parallel); });
        m.addItem ("Inverter",  [=] { node.setChild (IDs::Adaptor::Inverter); });
        m.addItem ("Resistor",  [=] { node.setChild (IDs::Leaf::Resistor); });
        m.addItem ("Capacitor", [=] { node.setChild (IDs::Leaf::Capacitor); });
        m.addItem ("Inductor",  [=] { node.setChild (IDs::Leaf::Inductor); });

        p.addSubMenu ("Set child", m);
    }

private:
    OnePort& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePortCell)
};

#endif // ONEPORTCELL_H_INCLUDED
