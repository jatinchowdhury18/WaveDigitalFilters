#ifndef IDEALISCELL_H_INCLUDED
#define IDEALISCELL_H_INCLUDED

#include "RootCell.h"
#include "SourceCell.h"
#include "../Nodes/IdealIs.h"

class IdealIsCell : public RootCell,
                    public SourceCell
{
public:
    IdealIsCell (IdealIs& node) :
        RootCell (node),
        SourceCell (node),
        node (node)
    {}
    virtual ~IdealIsCell() {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::red);

        g.setColour (Colours::black);
        g.drawFittedText ("Is", getLocalBounds(), Justification::centred, 1);
    }

    void getPopupMenu (PopupMenu& p) override
    {
        p.addItem ("Set current", {});
        SourceCell::getPopupMenu (p);
        RootCell::getPopupMenu (p);
    }

private:
    IdealIs& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IdealIsCell)
};

#endif // IDEALISCELL_H_INCLUDED
