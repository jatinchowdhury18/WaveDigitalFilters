#ifndef SERIESCELL_H_INCLUDED
#define SERIESCELL_H_INCLUDED

#include "Adaptors/Series.h"
#include "TwoPortCell.h"

class SeriesCell : public TwoPortCell
{
public:
    explicit SeriesCell (Series& node) : TwoPortCell (node),
                                         node (node)
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::white);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Series", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    Series& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeriesCell)
};

#endif // SERIESCELL_H_INCLUDED
