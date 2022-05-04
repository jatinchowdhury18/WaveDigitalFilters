#ifndef SERIES_H_INCLUDED
#define SERIES_H_INCLUDED

#include "TwoPort.h"

class Series : public TwoPort
{
public:
    Series();
    ~Series() override = default;

    bool prepare (double sampleRate) override;

private:
    std::unique_ptr<chowdsp::wdf::WDFSeries<double>> series;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Series)
};

#endif // SERIES_H_INCLUDED
