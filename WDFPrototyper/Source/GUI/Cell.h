#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED

#include "JuceHeader.h"

class Cell : public Component
{
public:
    Cell()
    {
        setSize (50, 50);
    }

    virtual ~Cell() {}

    virtual void getPopupMenu (PopupMenu& p) = 0;

    void mouseDown (const MouseEvent& e) override
    {
        PopupMenu p;
        getPopupMenu (p);

        p.show();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Cell)
};

#endif // CELL_H_INCLUDED
