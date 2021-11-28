#include "DiodePair.h"
#include "RootCells/DiodePairCell.h"

DiodePair::DiodePair() : Is (new Property ({ "Saturation Current", (float) 2.52e-9, 0.0f, 0.1f })),
                         vt (new Property ({ "Thermal Voltage", 0.02585f, 0.001f, 0.1f })),
                         numDiodes (new Property ({ "Num Diodes", 0.5f, 3.0f, 1.0f }))
{
    cell = std::make_unique<DiodePairCell> (*this);

    auto updateParams = [=]
    {
        if (auto* diodePair = dynamic_cast<chowdsp::WDF::DiodePair<double>*> (wdf.get()))
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
    if (child == nullptr || child->getWDF() == nullptr)
        return false;

    wdf = std::make_unique<chowdsp::WDF::DiodePair<double>> (child->getWDF(), (double) Is->value, (double) vt->value);

    return RootNode::prepare (sampleRate);
}

void DiodePair::childUpdated()
{
    wdf = std::make_unique<chowdsp::WDF::DiodePair<double>> (child->getWDF(), (double) Is->value, (double) vt->value);
}
