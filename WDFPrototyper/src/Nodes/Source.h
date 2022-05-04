#ifndef SOURCE_H_INCLUDED
#define SOURCE_H_INCLUDED

#include <juce_dsp/juce_dsp.h>

class Source
{
public:
    Source() = default;
    virtual ~Source() = default;

    void input (float sample)
    {
        inputFunc ((double) sample);
    }

    bool getInput() { return isInput; }
    virtual void setInput (bool input) { isInput = input; }

protected:
    std::function<void (double)> inputFunc;

    bool test = true;

private:
    bool isInput = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Source)
};

#endif // SOURCE_H_INCLUDED