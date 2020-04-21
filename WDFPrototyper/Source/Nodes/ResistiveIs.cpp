#include "ResistiveIs.h"
#include "../GUI/ResistiveIsCell.h"

ResistiveIs::ResistiveIs()
{
    cell = std::make_unique<ResistiveIsCell> (*this);
}

bool ResistiveIs::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
    {
        wdf = std::make_unique<WaveDigitalFilter::ResistiveCurrentSource> (1.0e9);

        inputFunc = [=] (double x)
        {
            dynamic_cast<WaveDigitalFilter::ResistiveCurrentSource*> (wdf.get())->setCurrent (x);
        };
    }

    return result;
}
