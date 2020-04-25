#ifndef RESISTIVEVSCELL_H_INCLUDED
#define RESISTIVEVSCELL_H_INCLUDED

#include "LeafCell.h"
#include "SourceCell.h"
#include "Leafs/ResistiveVs.h"

class ResistiveVsCell : public LeafCell,
                        public SourceCell
{
public:
    ResistiveVsCell (ResistiveVs& node) :
        LeafCell (node),
        SourceCell (node),
        node (node)
    {}
    virtual ~ResistiveVsCell() {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white);

        g.setColour (Colours::black);
        g.drawFittedText ("Vs", getLocalBounds(), Justification::centred, 1);
    }

    void getPopupMenu (PopupMenu& p) override
    {
        SourceCell::getPopupMenu (p);
        LeafCell::getPopupMenu (p);
    }

private:
    ResistiveVs& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResistiveVsCell)
};

#endif // RESISTIVEVSCELL_H_INCLUDED
