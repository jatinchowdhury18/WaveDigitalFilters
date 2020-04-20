#ifndef RESISTOR_H_INCLUDED
#define RESISTOR_H_INCLUDED

#include "Leaf.h"

class Resistor : public Leaf
{
public:
    Resistor();
    virtual ~Resistor() {}

    bool prepare (double sampleRate);

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Resistor)
};

#endif // RESISTOR_H_INCLUDED