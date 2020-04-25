#include "Cell.h"

Cell::Cell (const OwnedArray<Property>& props) :
    props (props)
{
    setSize (50, 50);
}

void Cell::getPopupMenu (PopupMenu& p)
{
    if (! props.isEmpty())
        p.addItem ("Properties", [=] { sendChangeMessage(); });
}

void Cell::mouseDown (const MouseEvent& e)
{
    if (e.mods.isPopupMenu())
    {
        PopupMenu p;
        getPopupMenu (p);

        p.show();
        return;
    }
}

void Cell::mouseDrag (const MouseEvent& e)
{
    auto pos = e.getEventRelativeTo (getParentComponent()).getPosition();
    setTopLeftPosition (pos.x - getWidth() / 2, pos.y - getHeight() / 2);
    getParentComponent()->repaint();
}

String Cell::getTooltip()
{
    if (props.isEmpty())
        return {};

    String result;
    for (auto p : props)
        result += p->name + ": " + String (p->value, 3, true) + "\n";

    return result;
}
