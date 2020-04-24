#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED

#include "JuceHeader.h"
#include "../Nodes/Property.h"

class Cell : public Component,
             public ChangeBroadcaster
{
public:
    Cell (const OwnedArray<Property>& props) :
        props (props)
    {
        setSize (50, 50);
    }

    virtual ~Cell() {}

    virtual void getPopupMenu (PopupMenu& p)
    {
        if (! props.isEmpty())
            p.addItem ("Properties", [=] { sendChangeMessage(); });
    }

    void mouseDown (const MouseEvent&) override
    {
        PopupMenu p;
        getPopupMenu (p);

        p.show();
    }

private:
    const OwnedArray<Property>& props;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Cell)
};

#endif // CELL_H_INCLUDED
