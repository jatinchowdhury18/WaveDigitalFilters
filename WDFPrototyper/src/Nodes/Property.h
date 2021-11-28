#ifndef PROPERTY_H_INCLUDED
#define PROPERTY_H_INCLUDED

#include "JuceHeader.h"

struct Property
{
    String name;
    float value;
    float minValue;
    float maxValue;
    std::function<void()> valueChanged = {};

    void setValue (float newVal)
    {
        if (newVal == value)
            return;

        value = jlimit (minValue, maxValue, newVal);
        valueChanged();
    }
};

#endif // PROPERTY_H_INCLUDED
