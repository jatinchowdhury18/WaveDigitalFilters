#ifndef IDEALVSCELL_H_INCLUDED
#define IDEALVSCELL_H_INCLUDED

#include "RootCell.h"
#include "Roots/IdealVs.h"
#include "SourceCell.h"

class IdealVsCell : public RootCell,
                    public SourceCell
{
public:
    explicit IdealVsCell (IdealVs& node) : RootCell (node),
                                           SourceCell (node),
                                           node (node)
    {
    }
    ~IdealVsCell() override = default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::red);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Vs", getLocalBounds(), juce::Justification::centred, 1);
    }

    void getPopupMenu (juce::PopupMenu& p) override
    {
        SourceCell::getPopupMenu (p);
        RootCell::getPopupMenu (p);
    }

private:
    IdealVs& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IdealVsCell)
};

#endif // IDEALVSCELL_H_INCLUDED
