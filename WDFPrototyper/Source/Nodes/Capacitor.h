#ifndef CAPACITOR_H_INCLUDED
#define CAPACITOR_H_INCLUDED

#include "Leaf.h"

class Capacitor : public Leaf
{
public:
    Capacitor();
    virtual ~Capacitor() {}

    bool prepare (double sampleRate) override;

private:
    Property* capacitance;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Capacitor)
};

#endif // CAPACITOR_H_INCLUDED
