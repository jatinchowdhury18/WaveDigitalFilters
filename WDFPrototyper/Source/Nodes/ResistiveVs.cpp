#include "ResistiveVs.h"
#include "../GUI/ResistiveVsCell.h"

ResistiveVs::ResistiveVs()
{
    cell = std::make_unique<ResistiveVsCell> (*this);
}

bool ResistiveVs::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
    {
        wdf = std::make_unique<WaveDigitalFilter::ResistiveVoltageSource> (1.0e-9);

        inputFunc = [=] (double x)
        {
            dynamic_cast<WaveDigitalFilter::ResistiveVoltageSource*> (wdf.get())->setVoltage (x);
        };
    }

    return result;
}
