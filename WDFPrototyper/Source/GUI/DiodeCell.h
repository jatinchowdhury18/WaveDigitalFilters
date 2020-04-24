#ifndef DIODECELL_H_INCLUDED
#define DIODECELL_H_INCLUDED

#include "RootCell.h"
#include "../Nodes//Diode.h"

class DiodeCell : public RootCell
{
public:
    DiodeCell (Diode& node) :
        RootCell (node),
        node (node)
    {}
    virtual ~DiodeCell() {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::red);

        g.setColour (Colours::black);
        g.drawFittedText ("Diode", getLocalBounds(), Justification::centred, 1);
    }

private:
    Diode& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodeCell)
};

#endif // DIODECELL_H_INCLUDED
