#ifndef INVERTERCELL_H_INCLUDED
#define INVERTERCELL_H_INCLUDED

#include "OnePortCell.h"
#include "Adaptors/Inverter.h"

class InverterCell : public OnePortCell
{
public:
    InverterCell (Inverter& node) :
        OnePortCell (node),
        node (node)
    {}

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white);

        g.setColour (Colours::black);
        g.drawFittedText ("Inv", getLocalBounds(), Justification::centred, 1);
    }

private:
    Inverter& node;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InverterCell)
};

#endif // INVERTERCELL_H_INCLUDED
