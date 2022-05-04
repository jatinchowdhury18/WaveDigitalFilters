#ifndef PARALLEL_H_INCLUDED
#define PARALLEL_H_INCLUDED

#include "TwoPort.h"

class Parallel : public TwoPort
{
public:
    Parallel();
    ~Parallel() override = default;

    bool prepare (double sampleRate) override;

private:
    std::unique_ptr<chowdsp::wdf::WDFParallel<double>> parallel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parallel)
};

#endif // PARALLEL_H_INCLUDED
