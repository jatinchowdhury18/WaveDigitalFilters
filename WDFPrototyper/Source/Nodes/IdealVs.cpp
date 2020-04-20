#include "IdealVs.h"
#include "../GUI/IdealVsCell.h"

IdealVs::IdealVs()
{
    cell = std::make_unique<IdealVsCell> (*this);
}

bool IdealVs::prepare (double sampleRate)
{
    wdf = std::make_unique<WaveDigitalFilter::IdealVoltageSource>();

    bool result = RootNode::prepare (sampleRate);

    if (result)
    {
        inputFunc = [=] (double x)
        {
            dynamic_cast<WaveDigitalFilter::IdealVoltageSource*> (wdf.get())->setVoltage (x);
        };
    }

    return result;
}
