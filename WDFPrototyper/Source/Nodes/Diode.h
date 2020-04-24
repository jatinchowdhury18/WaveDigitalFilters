#ifndef DIODE_H_INCLUDED
#define DIODE_H_INCLUDED

#include "RootNode.h"

class Diode : public RootNode
{
public:
    Diode();
    ~Diode() {}

    bool prepare (double sampleRate) override;

private:
    Property* Is; // saturation current
    Property* vt; // thermal voltage

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Diode)
};

#endif // DIODE_H_INCLUDED
