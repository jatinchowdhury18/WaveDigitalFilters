#ifndef IDEALISCELL_H_INCLUDED
#define IDEALISCELL_H_INCLUDED

#include "RootCell.h"
#include "Roots/IdealIs.h"
#include "SourceCell.h"

class IdealIsCell : public RootCell,
                    public SourceCell
{
public:
    explicit IdealIsCell (IdealIs& node) : RootCell (node),
                                           SourceCell (node),
                                           node (node)
    {
    }
    ~IdealIsCell() override = default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::red);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Is", getLocalBounds(), juce::Justification::centred, 1);
    }

    void getPopupMenu (juce::PopupMenu& p) override
    {
        SourceCell::getPopupMenu (p);
        RootCell::getPopupMenu (p);
    }

private:
    IdealIs& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IdealIsCell)
};

#endif // IDEALISCELL_H_INCLUDED
