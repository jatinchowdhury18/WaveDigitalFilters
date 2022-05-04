#ifndef DIODEPAIRCELL_H_INCLUDED
#define DIODEPAIRCELL_H_INCLUDED

#include "RootCell.h"
#include "Roots/DiodePair.h"

class DiodePairCell : public RootCell
{
public:
    explicit DiodePairCell (DiodePair& node) : RootCell (node),
                                               node (node)
    {
    }
    ~DiodePairCell() override = default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::red);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("DP", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    DiodePair& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodePairCell)
};

#endif // DIODEPAIRCELL_H_INCLUDED
