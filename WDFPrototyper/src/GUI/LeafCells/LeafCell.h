#ifndef LEAFCELL_H_INCLUDED
#define LEAFCELL_H_INCLUDED

#include "Cell.h"
#include "Leafs/Leaf.h"

class LeafCell : public Cell
{
public:
    explicit LeafCell (Leaf& node) : Cell (node.getProps()),
                                     node (node)
    {
    }
    ~LeafCell() override = default;

    void getPopupMenu (juce::PopupMenu& p) override
    {
        juce::PopupMenu m;
        m.addItem ("Resistor", [=]
                   { node.replaceNode (IDs::Leaf::Resistor); });
        m.addItem ("Inductor", [=]
                   { node.replaceNode (IDs::Leaf::Inductor); });
        m.addItem ("Capacitor", [=]
                   { node.replaceNode (IDs::Leaf::Capacitor); });
        m.addItem ("Voltage Source", [=]
                   { node.replaceNode (IDs::Leaf::ResistiveVs); });
        m.addItem ("Current Source", [=]
                   { node.replaceNode (IDs::Leaf::ResistiveIs); });

        p.addSubMenu ("Replace Node", m);

        p.addItem ("Probe", true, node.getProbe(), [=]
                   { node.setProbe (true); });

        Cell::getPopupMenu (p);
    }

private:
    Leaf& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeafCell)
};

#endif // LEAFCELL_H_INCLUDED