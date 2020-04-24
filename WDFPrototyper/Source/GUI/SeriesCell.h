#ifndef SERIESCELL_H_INCLUDED
#define SERIESCELL_H_INCLUDED

#include "TwoPortCell.h"
#include "../Nodes/Series.h"

class SeriesCell : public TwoPortCell
{
public:
    SeriesCell (Series& node) :
        TwoPortCell (node),
        node (node)
    {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white);

        g.setColour (Colours::black);
        g.drawFittedText ("Series", getLocalBounds(), Justification::centred, 1);
    }

private:
    Series& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR  (SeriesCell)
};

#endif // SERIESCELL_H_INCLUDED
