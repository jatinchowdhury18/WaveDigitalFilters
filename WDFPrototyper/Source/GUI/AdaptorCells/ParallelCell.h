#ifndef PARALLELCELL_H_INCLUDED
#define PARALLELCELL_H_INCLUDED

#include "TwoPortCell.h"
#include "Adaptors/Parallel.h"

class ParallelCell : public TwoPortCell
{
public:
    ParallelCell (Parallel& node) :
        TwoPortCell (node),
        node (node)
    {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white);

        g.setColour (Colours::black);
        g.drawFittedText ("Par", getLocalBounds(), Justification::centred, 1);
    }

private:
    Parallel& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParallelCell)
};

#endif // PARALLELCELL_H_INCLUDED
