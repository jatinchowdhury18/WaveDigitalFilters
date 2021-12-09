#ifndef CAPACITOR_H_INCLUDED
#define CAPACITOR_H_INCLUDED

#include "Leaf.h"

class Capacitor : public Leaf
{
public:
    Capacitor();
    ~Capacitor() override = default;

    bool prepare (double sampleRate) override;

private:
    Property* capacitance;

    std::unique_ptr<chowdsp::WDF::Capacitor<double>> cap;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Capacitor)
};

#endif // CAPACITOR_H_INCLUDED
