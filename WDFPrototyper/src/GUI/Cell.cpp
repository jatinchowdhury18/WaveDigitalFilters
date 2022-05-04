#include "Cell.h"

Cell::Cell (const juce::OwnedArray<Property>& props) : props (props)
{
    setSize (50, 50);
}

void Cell::getPopupMenu (juce::PopupMenu& p)
{
    if (! props.isEmpty())
        p.addItem ("Properties", [=]
                   { sendChangeMessage(); });
}

void Cell::mouseDown (const juce::MouseEvent& e)
{
    if (e.mods.isPopupMenu())
    {
        juce::PopupMenu p;
        getPopupMenu (p);

        p.showMenuAsync (juce::PopupMenu::Options());
    }
}

void Cell::mouseDrag (const juce::MouseEvent& e)
{
    auto pos = e.getEventRelativeTo (getParentComponent()).getPosition();
    setTopLeftPosition (pos.x - getWidth() / 2, pos.y - getHeight() / 2);
    getParentComponent()->repaint();
}

juce::String Cell::getTooltip()
{
    if (props.isEmpty())
        return {};

    juce::String result;
    for (auto p : props)
        result += p->name + ": " + juce::String (p->value, 3, true) + "\n";

    return result;
}
