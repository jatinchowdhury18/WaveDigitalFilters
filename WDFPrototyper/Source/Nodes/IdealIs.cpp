#include "IdealIs.h"
#include "../GUI/IdealIsCell.h"

IdealIs::IdealIs()
{
    cell = std::make_unique<IdealIsCell> (*this);
}

bool IdealIs::prepare (double sampleRate)
{
    wdf = std::make_unique<WaveDigitalFilter::IdealCurrentSource>();

    bool result = RootNode::prepare (sampleRate);

    if (result)
    {
        inputFunc = [=] (double x)
        {
            dynamic_cast<WaveDigitalFilter::IdealCurrentSource*> (wdf.get())->setCurrent (x);
        };
    }

    return result;
}
