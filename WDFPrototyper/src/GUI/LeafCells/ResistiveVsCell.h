#ifndef RESISTIVEVSCELL_H_INCLUDED
#define RESISTIVEVSCELL_H_INCLUDED

#include "LeafCell.h"
#include "Leafs/ResistiveVs.h"
#include "SourceCell.h"

class ResistiveVsCell : public LeafCell,
                        public SourceCell
{
public:
    explicit ResistiveVsCell (ResistiveVs& node) : LeafCell (node),
                                                   SourceCell (node),
                                                   node (node)
    {
    }
    ~ResistiveVsCell() override = default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::white);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Vs", getLocalBounds(), juce::Justification::centred, 1);
    }

    void getPopupMenu (juce::PopupMenu& p) override
    {
        SourceCell::getPopupMenu (p);
        LeafCell::getPopupMenu (p);
    }

private:
    ResistiveVs& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResistiveVsCell)
};

#endif // RESISTIVEVSCELL_H_INCLUDED
