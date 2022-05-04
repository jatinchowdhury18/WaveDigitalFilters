#ifndef RESISTORCELL_H_INCLUDED
#define RESISTORCELL_H_INCLUDED

#include "LeafCell.h"
#include "Leafs/Resistor.h"

class ResistorCell : public LeafCell
{
public:
    explicit ResistorCell (Resistor& node) : LeafCell (node),
                                             node (node)
    {
    }
    ~ResistorCell() override = default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (node.getProbe() ? juce::Colours::greenyellow : juce::Colours::white);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Res", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    Resistor& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResistorCell)
};

#endif // RESISTORCELL_H_INCLUDED