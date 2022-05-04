#ifndef RESISTOR_H_INCLUDED
#define RESISTOR_H_INCLUDED

#include "Leaf.h"

class Resistor : public Leaf
{
public:
    Resistor();
    ~Resistor() override = default;

    bool prepare (double sampleRate) override;

private:
    Property* resistor;

    std::unique_ptr<chowdsp::wdf::Resistor<double>> res;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Resistor)
};

#endif // RESISTOR_H_INCLUDED