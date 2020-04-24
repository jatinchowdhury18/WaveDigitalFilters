#ifndef INDUCTORCELL_H_INCLUDED
#define INDUCTORCELL_H_INCLUDED

#include "LeafCell.h"
#include "../Nodes/Inductor.h"

class InductorCell : public LeafCell
{
public:
    InductorCell (Inductor& node) :
        LeafCell (node),
        node (node)
    {}
    virtual ~InductorCell() {}

    void paint (Graphics& g) override
    {
        g.fillAll (node.getProbe() ? Colours::greenyellow : Colours::white);

        g.setColour (Colours::black);
        g.drawFittedText ("Ind", getLocalBounds(), Justification::centred, 1);
    }

private:
    Inductor& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InductorCell)
};

#endif // INDUCTORCELL_H_INCLUDED
