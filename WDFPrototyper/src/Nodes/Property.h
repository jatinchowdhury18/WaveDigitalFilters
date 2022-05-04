#ifndef PROPERTY_H_INCLUDED
#define PROPERTY_H_INCLUDED

#include <juce_dsp/juce_dsp.h>

struct Property
{
    juce::String name;
    float value;
    float minValue;
    float maxValue;
    std::function<void()> valueChanged = {};

    void setValue (float newVal)
    {
        if (newVal == value)
            return;

        value = juce::jlimit (minValue, maxValue, newVal);
        valueChanged();
    }
};

#endif // PROPERTY_H_INCLUDED
