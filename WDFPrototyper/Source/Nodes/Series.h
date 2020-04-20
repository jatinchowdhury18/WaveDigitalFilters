#ifndef SERIES_H_INCLUDED
#define SERIES_H_INCLUDED

#include "TwoPort.h"

class Series : public TwoPort
{
public:
    Series();
    virtual ~Series() {}

    bool prepare (double sampleRate);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Series)
};

#endif // SERIES_H_INCLUDED
