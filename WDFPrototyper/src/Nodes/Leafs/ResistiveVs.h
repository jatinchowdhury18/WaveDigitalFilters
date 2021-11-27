#ifndef RESISTIVEVS_H_INCLUDED
#define RESISTIVEVS_H_INCLUDED

#include "Leaf.h"
#include "Source.h"

class ResistiveVs : public Leaf,
                    public Source
{
public:
    ResistiveVs();
    virtual ~ResistiveVs();

    bool prepare (double sampleRate) override;

    void setInput (bool input) override;
    void setVoltage (float voltageValue);
    void setResistance (float resValue);

private:
    Property* voltage;
    Property* resistance;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResistiveVs)
};

#endif // RESISTIVEVS_H_INCLUDED
