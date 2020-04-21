#ifndef DIODEPAIR_H_INCLUDED
#define DIODEPAIR_H_INCLUDED

#include "RootNode.h"

class DiodePair : public RootNode
{
public:
    DiodePair();
    ~DiodePair() {}

    bool prepare (double sampleRate);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiodePair)
};

#endif // DIODEPAIR_H_INCLUDED
