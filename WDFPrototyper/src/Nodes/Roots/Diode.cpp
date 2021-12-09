#include "Diode.h"
#include "RootCells/DiodeCell.h"

Diode::Diode() : Is (new Property ({ "Saturation Current", (float) 2.52e-9, 0.0f, 0.1f })),
                 vt (new Property ({ "Thermal Voltage", 0.02585f, 0.001f, 0.1f })),
                 numDiodes (new Property ({ "Num Diodes", 0.5f, 3.0f, 1.0f }))
{
    cell = std::make_unique<DiodeCell> (*this);

    auto updateParams = [=]
    {
        if (diode != nullptr)
            diode->setDiodeParameters ((double) Is->value, (double) vt->value, (double) numDiodes->value);
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

bool Diode::prepare (double sampleRate)
{
    if (child == nullptr)
        return false;

    if (! RootNode::prepare (sampleRate))
        return false;

    diode = std::make_unique<chowdsp::WDF::Diode<double>> (child->getWDF(), (double) Is->value, (double) vt->value, (double) numDiodes->value);
    wdf = diode.get();
}
