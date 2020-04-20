#ifndef RESISTORCELL_H_INCLUDED
#define RESISTORCELL_H_INCLUDED

#include "LeafCell.h"
#include "../Nodes/Resistor.h"

class ResistorCell : public LeafCell
{
public:
    ResistorCell (Resistor& node) :
        LeafCell (node),
        node (node)
    {}
    virtual ~ResistorCell() {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white);

        g.setColour (Colours::black);
        g.drawFittedText ("Res", getLocalBounds(), Justification::centred, 1);
    }

private:
    Resistor& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResistorCell)
};

#endif // RESISTORCELL_H_INCLUDED