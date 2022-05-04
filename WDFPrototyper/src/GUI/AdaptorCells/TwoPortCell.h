#ifndef TWOPORTCELL_H_INCLUDED
#define TWOPORTCELL_H_INCLUDED

#include "Adaptors/TwoPort.h"
#include "Cell.h"

class TwoPortCell : public Cell
{
public:
    explicit TwoPortCell (TwoPort& node) : Cell (node.getProps()),
                                           node (node)
    {
    }
    ~TwoPortCell() override = default;

    void getPopupMenu (juce::PopupMenu& p) override
    {
        auto addChildren = [=] (juce::PopupMenu& m, int idx)
        {
            m.addItem ("Series", [=]
                       { node.setChild (idx, IDs::Adaptor::Series); });
            m.addItem ("Parallel", [=]
                       { node.setChild (idx, IDs::Adaptor::Parallel); });
            m.addItem ("Inverter", [=]
                       { node.setChild (idx, IDs::Adaptor::Inverter); });
            m.addItem ("Resistor", [=]
                       { node.setChild (idx, IDs::Leaf::Resistor); });
            m.addItem ("Capacitor", [=]
                       { node.setChild (idx, IDs::Leaf::Capacitor); });
            m.addItem ("Inductor", [=]
                       { node.setChild (idx, IDs::Leaf::Inductor); });
            m.addItem ("Voltage Source", [=]
                       { node.setChild (idx, IDs::Leaf::ResistiveVs); });
            m.addItem ("Current Source", [=]
                       { node.setChild (idx, IDs::Leaf::ResistiveIs); });
        };

        juce::PopupMenu childrenMenu1;
        addChildren (childrenMenu1, 0);

        juce::PopupMenu childrenMenu2;
        addChildren (childrenMenu2, 1);

        p.addSubMenu ("Set child 1", childrenMenu1);
        p.addSubMenu ("Set child 2", childrenMenu2);

        juce::PopupMenu m;
        m.addItem ("Series", [=]
                   { node.replaceNode (IDs::Adaptor::Series); });
        m.addItem ("Parallel", [=]
                   { node.replaceNode (IDs::Adaptor::Parallel); });

        p.addSubMenu ("Replace Node", m);

        Cell::getPopupMenu (p);
    }

private:
    TwoPort& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoPortCell)
};

#endif // TWOPORTCELL_H_INCLUDED
