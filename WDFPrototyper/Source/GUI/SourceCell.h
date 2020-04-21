#ifndef SOURCECELL_H_INCLUDED
#define SOURCECELL_H_INCLUDED

#include "Cell.h"

class SourceCell
{
public:
    SourceCell (Source& source) :
        source (source)
    {}
    ~SourceCell() {}

    void getPopupMenu (PopupMenu& p)
    {
        p.addItem ("Input", true, source.getInput(), [=] { source.setInput (true); });
    }

private:
    Source& source;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceCell)
};

#endif // SOURCECELL_H_INCLUDED
