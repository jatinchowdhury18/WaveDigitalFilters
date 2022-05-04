#ifndef INVERTERCELL_H_INCLUDED
#define INVERTERCELL_H_INCLUDED

#include "Adaptors/Inverter.h"
#include "OnePortCell.h"

class InverterCell : public OnePortCell
{
public:
    explicit InverterCell (Inverter& node) : OnePortCell (node),
                                             node (node)
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::white);

        g.setColour (juce::Colours::black);
        g.drawFittedText ("Inv", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    Inverter& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InverterCell)
};

#endif // INVERTERCELL_H_INCLUDED
