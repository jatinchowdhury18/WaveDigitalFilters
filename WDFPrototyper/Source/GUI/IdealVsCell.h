#ifndef IDEALVSCELL_H_INCLUDED
#define IDEALVSCELL_H_INCLUDED

#include "RootCell.h"
#include "SourceCell.h"
#include "../Nodes/IdealVs.h"

class IdealVsCell : public RootCell,
                    public SourceCell
{
public:
    IdealVsCell (IdealVs& node) :
        RootCell (node),
        SourceCell (node),
        node (node)
    {}
    virtual ~IdealVsCell() {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::red);
        
        g.setColour (Colours::black);
        g.drawFittedText ("Vs", getLocalBounds(), Justification::centred, 1);
    }

    void getPopupMenu (PopupMenu& p) override
    {
        p.addItem ("Set voltage", {});
        SourceCell::getPopupMenu (p);
        RootCell::getPopupMenu (p);
    }

private:
    IdealVs& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IdealVsCell)
};

#endif // IDEALVSCELL_H_INCLUDED
