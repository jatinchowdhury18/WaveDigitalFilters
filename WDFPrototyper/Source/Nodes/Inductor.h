#ifndef INDUCTOR_H_INCLUDED
#define INDUCTOR_H_INCLUDED

#include "Leaf.h"

class Inductor : public Leaf
{
public:
    Inductor();
    virtual ~Inductor() {}

    bool prepare (double sampleRate) override;

private:
    Property* inductance;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Inductor)
};

#endif // INDUCTOR_H_INCLUDED
