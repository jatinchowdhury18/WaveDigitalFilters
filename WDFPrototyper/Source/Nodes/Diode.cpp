#include "Diode.h"
#include "../GUI/DiodeCell.h"

Diode::Diode()
{
    cell = std::make_unique<DiodeCell> (*this);
}

bool Diode::prepare (double sampleRate)
{
    wdf = std::make_unique<WaveDigitalFilter::Diode> (2.52e-9, 0.02585);

    return RootNode::prepare (sampleRate);
}
