#ifndef CAPACITORCELL_H_INCLUDED
#define CAPACITORCELL_H_INCLUDED

#include "LeafCell.h"
#include "../Nodes/Capacitor.h"

class CapacitorCell : public LeafCell
{
public:
    CapacitorCell (Capacitor& node) :
        LeafCell (node),
        node (node)
    {}
    virtual ~CapacitorCell() {}

    void paint (Graphics& g) override
    {
        g.fillAll (node.getProbe() ? Colours::greenyellow : Colours::white);

        g.setColour (Colours::black);
        g.drawFittedText ("Cap", getLocalBounds(), Justification::centred, 1);
    }

private:
    Capacitor& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CapacitorCell)
};

#endif // CAPACITORCELL_H_INCLUDED
