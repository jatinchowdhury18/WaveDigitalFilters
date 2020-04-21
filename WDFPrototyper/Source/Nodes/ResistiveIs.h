#ifndef RESISTIVEIS_H_INCLUDED
#define RESISTIVEIS_H_INCLUDED

#include "Leaf.h"
#include "Source.h"

class ResistiveIs : public Leaf,
                    public Source
{
public:
    ResistiveIs();
    virtual ~ResistiveIs() {}

    bool prepare (double sampleRate);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResistiveIs)
};

#endif // RESISTIVEIS_H_INCLUDED
