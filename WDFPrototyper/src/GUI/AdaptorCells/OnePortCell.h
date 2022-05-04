#ifndef ONEPORTCELL_H_INCLUDED
#define ONEPORTCELL_H_INCLUDED

#include "Adaptors/OnePort.h"
#include "Cell.h"

class OnePortCell : public Cell
{
public:
    explicit OnePortCell (OnePort& node) : Cell (node.getProps()),
                                           node (node)
    {
    }
    ~OnePortCell() override = default;

    void getPopupMenu (juce::PopupMenu& p) override
    {
        juce::PopupMenu m;

        m.addItem ("Series", [=]
                   { node.setChild (IDs::Adaptor::Series); });
        m.addItem ("Parallel", [=]
                   { node.setChild (IDs::Adaptor::Parallel); });
        m.addItem ("Inverter", [=]
                   { node.setChild (IDs::Adaptor::Inverter); });
        m.addItem ("Resistor", [=]
                   { node.setChild (IDs::Leaf::Resistor); });
        m.addItem ("Capacitor", [=]
                   { node.setChild (IDs::Leaf::Capacitor); });
        m.addItem ("Inductor", [=]
                   { node.setChild (IDs::Leaf::Inductor); });
        m.addItem ("Voltage Source", [=]
                   { node.setChild (IDs::Leaf::ResistiveVs); });
        m.addItem ("Current Source", [=]
                   { node.setChild (IDs::Leaf::ResistiveIs); });

        p.addSubMenu ("Set child", m);

        Cell::getPopupMenu (p);
    }

private:
    OnePort& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePortCell)
};

#endif // ONEPORTCELL_H_INCLUDED
