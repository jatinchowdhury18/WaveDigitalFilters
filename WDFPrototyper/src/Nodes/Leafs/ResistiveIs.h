#ifndef RESISTIVEIS_H_INCLUDED
#define RESISTIVEIS_H_INCLUDED

#include "Leaf.h"
#include "Source.h"

class ResistiveIs : public Leaf,
                    public Source
{
public:
    ResistiveIs();
    ~ResistiveIs() override;

    bool prepare (double sampleRate) override;

    void setInput (bool input) override;
    void setCurrent (float currentValue);
    void setResistance (float resValue);

private:
    Property* current;
    Property* resistance;

    std::unique_ptr<chowdsp::wdf::ResistiveCurrentSource<double>> resIs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResistiveIs)
};

#endif // RESISTIVEIS_H_INCLUDED
