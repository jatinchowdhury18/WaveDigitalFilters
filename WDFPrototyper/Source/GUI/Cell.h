#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED

#include "JuceHeader.h"
#include "../Nodes/Property.h"

class Cell : public Component,
             public ChangeBroadcaster,
             public TooltipClient
{
public:
    Cell (const OwnedArray<Property>& props);
    virtual ~Cell() {}

    virtual void getPopupMenu (PopupMenu& p);

    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;

    String getTooltip() override;

private:
    const OwnedArray<Property>& props;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Cell)
};

#endif // CELL_H_INCLUDED
