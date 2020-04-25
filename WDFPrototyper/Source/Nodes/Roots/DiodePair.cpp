#include "DiodePair.h"
#include "RootCells/DiodePairCell.h"

DiodePair::DiodePair() :
    Is (new Property ({"Saturation Current", (float) 2.52e-9, 0.0f, 0.1f})),
    vt (new Property ({"Thermal Voltage", 0.02585f, 0.001f, 0.1f}))
{
    cell = std::make_unique<DiodePairCell> (*this);

    Is->valueChanged = [=]
    {
        wdf = std::make_unique<WaveDigitalFilter::DiodePair>
            ((double) Is->value, (double) vt->value);
    };
    props.add (Is);

    vt->valueChanged = [=]
    {
        wdf = std::make_unique<WaveDigitalFilter::DiodePair>
            ((double) Is->value, (double) vt->value);
    };
    props.add (vt);
}

bool DiodePair::prepare (double sampleRate)
{
    wdf = std::make_unique<WaveDigitalFilter::DiodePair> ((double) Is->value, (double) vt->value);

    return RootNode::prepare (sampleRate);
}
