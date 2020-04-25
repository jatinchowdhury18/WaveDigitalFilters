#ifndef DIODEPAIRCELL_H_INCLUDED
#define DIODEPAIRCELL_H_INCLUDED

#include "RootCell.h"
#include "Roots/DiodePair.h"

class DiodePairCell : public RootCell
{
public:
    DiodePairCell (DiodePair& node) :
        RootCell (node),
        node (node)
    {}
    virtual ~DiodePairCell() {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::red);

        g.setColour (Colours::black);
        g.drawFittedText ("DP", getLocalBounds(), Justification::centred, 1);
    }

private:
    DiodePair& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodePairCell)
};

#endif // DIODEPAIRCELL_H_INCLUDED
