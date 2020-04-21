#include "DiodePair.h"
#include "../GUI/DiodePairCell.h"

DiodePair::DiodePair()
{
    cell = std::make_unique<DiodePairCell> (*this);
}

bool DiodePair::prepare (double sampleRate)
{
    wdf = std::make_unique<WaveDigitalFilter::DiodePair> (2.52e-9, 0.02585);

    return RootNode::prepare (sampleRate);
}
