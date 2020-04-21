#include "Inductor.h"
#include "../GUI/InductorCell.h"

Inductor::Inductor()
{
    cell = std::make_unique<InductorCell> (*this);
}

bool Inductor::prepare (double sampleRate)
{
    bool result =  Leaf::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<WaveDigitalFilter::Inductor> (1.0e-3, sampleRate);

    return result;
}
