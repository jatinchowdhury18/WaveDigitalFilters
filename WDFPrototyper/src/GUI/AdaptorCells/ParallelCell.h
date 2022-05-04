#ifndef PARALLELCELL_H_INCLUDED
#define PARALLELCELL_H_INCLUDED

#include "Adaptors/Parallel.h"
#include "TwoPortCell.h"

class ParallelCell : public TwoPortCell
{
public:
    explicit ParallelCell (Parallel& node) : TwoPortCell (node),
                                             node (node)
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::white);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Par", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    Parallel& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParallelCell)
};

#endif // PARALLELCELL_H_INCLUDED
