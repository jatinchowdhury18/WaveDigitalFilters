#include "Parallel.h"
#include "AdaptorCells/ParallelCell.h"

Parallel::Parallel()
{
    cell = std::make_unique<ParallelCell> (*this);
}

bool Parallel::prepare (double sampleRate)
{
    bool result = TwoPort::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<WaveDigitalFilter::WDFParallel> (child[0]->getWDF(), child[1]->getWDF());

    return result;
}
