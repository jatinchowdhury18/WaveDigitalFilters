#ifndef IDEALIS_H_INCLUDED
#define IDEALIS_H_INCLUDED

#include "RootNode.h"
#include "Source.h"

class IdealIs : public RootNode,
                public Source
{
public:
    IdealIs();
    ~IdealIs() override;

    bool prepare (double sampleRate) override;

    void setInput (bool input) override;
    void setCurrent (float currentValue);

private:
    Property* current;

    std::unique_ptr<chowdsp::WDF::IdealCurrentSource<double>> idealIs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IdealIs)
};

#endif // IDEALIS_H_INCLUDED
