#include "Inductor.h"
#include "LeafCells/InductorCell.h"

Inductor::Inductor() : inductance (new Property ({ "Inductance", 0.001f, (float) 1.0e-6, 20.0f }))
{
    cell = std::make_unique<InductorCell> (*this);

    inductance->valueChanged = [=]
    {
        if (ind != nullptr)
            ind->setInductanceValue (inductance->value);
    };
    props.add (inductance);
}

bool Inductor::prepare (double sampleRate)
{
    if (! Leaf::prepare (sampleRate))
        return false;

    ind = std::make_unique<chowdsp::wdf::Inductor<double>> (inductance->value, sampleRate);
    wdf = ind.get();

    return true;
}
