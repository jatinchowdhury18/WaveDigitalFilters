#ifndef RESISTIVEISCELL_H_INCLUDED
#define RESISTIVEISCELL_H_INCLUDED

#include "LeafCell.h"
#include "SourceCell.h"
#include "Leafs/ResistiveIs.h"

class ResistiveIsCell : public LeafCell,
                        public SourceCell
{
public:
    ResistiveIsCell (ResistiveIs& node) :
        LeafCell (node),
        SourceCell (node),
        node (node)
    {}
    virtual ~ResistiveIsCell() {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white);

        g.setColour (Colours::black);
        g.drawFittedText ("Is", getLocalBounds(), Justification::centred, 1);
    }

    void getPopupMenu (PopupMenu& p) override
    {
        SourceCell::getPopupMenu (p);
        LeafCell::getPopupMenu (p);
    }
    
private:
    ResistiveIs& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResistiveIsCell)
};

#endif // RESISTIVEISCELL_H_INCLUDED
