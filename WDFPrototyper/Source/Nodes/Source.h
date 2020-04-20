#ifndef SOURCE_H_INCLUDED
#define SOURCE_H_INCLUDED

#include "JuceHeader.h"

class Source
{
public:
    Source() {}
    virtual ~Source() {}

    void input (float sample)
    {
        inputFunc ((double) sample);
    }

    bool getInput() { return isInput; }
    void setInput (bool input) { isInput = input; }

protected:
    std::function<void (double)> inputFunc;

    bool test = true;

private:
    bool isInput = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Source)
};

#endif // SOURCE_H_INCLUDED