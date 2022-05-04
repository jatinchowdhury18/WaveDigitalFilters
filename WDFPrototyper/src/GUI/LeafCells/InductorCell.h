#ifndef INDUCTORCELL_H_INCLUDED
#define INDUCTORCELL_H_INCLUDED

#include "LeafCell.h"
#include "Leafs/Inductor.h"

class InductorCell : public LeafCell
{
public:
    explicit InductorCell (Inductor& node) : LeafCell (node),
                                             node (node)
    {
    }
    ~InductorCell() override = default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (node.getProbe() ? juce::Colours::greenyellow : juce::Colours::white);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Ind", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    Inductor& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InductorCell)
};

#endif // INDUCTORCELL_H_INCLUDED
