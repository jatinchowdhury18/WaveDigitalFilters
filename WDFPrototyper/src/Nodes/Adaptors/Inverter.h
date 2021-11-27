#ifndef INVERTER_H_INCLUDED
#define INVERTER_H_INCLUDED

#include "OnePort.h"

class Inverter : public OnePort
{
public:
    Inverter();
    virtual ~Inverter() {}

    bool prepare (double sampleRate) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Inverter)
};

#endif // INVERTER_H_INCLUDED
