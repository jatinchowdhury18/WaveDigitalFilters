#ifndef IDEALVS_H_INCLUDED
#define IDEALVS_H_INCLUDED

#include "RootNode.h"
#include "Source.h"

class IdealVs : public RootNode,
                public Source
{
public:
    IdealVs();
    ~IdealVs() override;

    bool prepare (double sampleRate) override;

    void setInput (bool input) override;
    void setVoltage (float voltageValue);

private:
    Property* voltage;

    std::unique_ptr<chowdsp::WDF::IdealVoltageSource<double>> idealVs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IdealVs)
};

#endif // IDEALVS_H_INCLUDED
