#ifndef DIODEPAIR_H_INCLUDED
#define DIODEPAIR_H_INCLUDED

#include "RootNode.h"

class DiodePair : public RootNode
{
public:
    DiodePair();
    ~DiodePair() {}

    bool prepare (double sampleRate) override;
    void childUpdated() override;

private:
    Property* Is; // saturation current
    Property* vt; // thermal voltage
    Property* numDiodes; // number of diodes

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodePair)
};

#endif // DIODEPAIR_H_INCLUDED
