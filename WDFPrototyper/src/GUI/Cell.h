#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED

#include <juce_gui_basics/juce_gui_basics.h>

#include "Property.h"

class Cell : public juce::Component,
             public juce::ChangeBroadcaster,
             public juce::TooltipClient
{
public:
    explicit Cell (const juce::OwnedArray<Property>& props);
    virtual ~Cell() = default;

    virtual void getPopupMenu (juce::PopupMenu& p);

    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;

    juce::String getTooltip() override;

private:
    const juce::OwnedArray<Property>& props;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Cell)
};

#endif // CELL_H_INCLUDED
