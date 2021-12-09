#include "DiodePair.h"
#include "RootCells/DiodePairCell.h"

DiodePair::DiodePair() : Is (new Property ({ "Saturation Current", (float) 2.52e-9, 0.0f, 0.1f })),
                         vt (new Property ({ "Thermal Voltage", 0.02585f, 0.001f, 0.1f })),
                         numDiodes (new Property ({ "Num Diodes", 1.0f, 0.5f, 3.0f }))
{
    cell = std::make_unique<DiodePairCell> (*this);

    auto updateParams = [=]
    {
        if (diodePair != nullptr)
            diodePair->setDiodeParameters ((double) Is->value, (double) vt->value, (double) numDiodes->value);
    };

    Is->valueChanged = [=]
    { updateParams(); };
    props.add (Is);

    vt->valueChanged = [=]
    { updateParams(); };
    props.add (vt);

    numDiodes->valueChanged = [=]
    { updateParams(); };
    props.add (numDiodes);
}

bool DiodePair::prepare (double sampleRate)
{
    if (child == nullptr)
        return false;

    if (! RootNode::prepare (sampleRate))
        return false;

    diodePair = std::make_unique<chowdsp::WDF::DiodePair<double>> (child->getWDF(), (double) Is->value, (double) vt->value);
    wdf = diodePair.get();

    return true;
}
