#ifndef PARALLEL_H_INCLUDED
#define PARALLEL_H_INCLUDED

#include "TwoPort.h"

class Parallel : public TwoPort
{
public:
    Parallel();
    virtual ~Parallel() {}

    bool prepare (double sampleRate) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parallel)
};

#endif // PARALLEL_H_INCLUDED
