#include "Inductor.h"
#include "LeafCells/InductorCell.h"

Inductor::Inductor() :
    inductance (new Property ({"Inductance", 0.001f, (float) 1.0e-6, 20.0f}))
{
    cell = std::make_unique<InductorCell> (*this);

    inductance->valueChanged = [=]
    {
        if (auto ind = dynamic_cast<WaveDigitalFilter::Inductor*> (wdf.get()))
            ind->setInductanceValue (inductance->value);
    };
    props.add (inductance);
}

bool Inductor::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<WaveDigitalFilter::Inductor> (inductance->value, sampleRate);

    return result;
}
