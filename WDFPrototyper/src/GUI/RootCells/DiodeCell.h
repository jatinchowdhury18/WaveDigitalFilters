#ifndef DIODECELL_H_INCLUDED
#define DIODECELL_H_INCLUDED

#include "RootCell.h"
#include "Roots/Diode.h"

class DiodeCell : public RootCell
{
public:
    explicit DiodeCell (Diode& node) : RootCell (node),
                                       node (node)
    {
    }
    ~DiodeCell() override = default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::red);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Diode", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    Diode& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodeCell)
};

#endif // DIODECELL_H_INCLUDED
