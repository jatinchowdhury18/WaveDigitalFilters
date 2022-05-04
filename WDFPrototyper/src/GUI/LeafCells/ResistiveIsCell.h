#ifndef RESISTIVEISCELL_H_INCLUDED
#define RESISTIVEISCELL_H_INCLUDED

#include "LeafCell.h"
#include "Leafs/ResistiveIs.h"
#include "SourceCell.h"

class ResistiveIsCell : public LeafCell,
                        public SourceCell
{
public:
    explicit ResistiveIsCell (ResistiveIs& node) : LeafCell (node),
                                                   SourceCell (node),
                                                   node (node)
    {
    }
    ~ResistiveIsCell() override = default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::white);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Is", getLocalBounds(), juce::Justification::centred, 1);
    }

    void getPopupMenu (juce::PopupMenu& p) override
    {
        SourceCell::getPopupMenu (p);
        LeafCell::getPopupMenu (p);
    }

private:
    ResistiveIs& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResistiveIsCell)
};

#endif // RESISTIVEISCELL_H_INCLUDED
