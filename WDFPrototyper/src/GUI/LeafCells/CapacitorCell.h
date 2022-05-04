#ifndef CAPACITORCELL_H_INCLUDED
#define CAPACITORCELL_H_INCLUDED

#include "LeafCell.h"
#include "Leafs/Capacitor.h"

class CapacitorCell : public LeafCell
{
public:
    explicit CapacitorCell (Capacitor& node) : LeafCell (node),
                                               node (node)
    {
    }
    ~CapacitorCell() override = default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (node.getProbe() ? juce::Colours::greenyellow : juce::Colours::white);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Cap", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    Capacitor& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CapacitorCell)
};

#endif // CAPACITORCELL_H_INCLUDED
