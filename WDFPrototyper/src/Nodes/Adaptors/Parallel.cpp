#include "Parallel.h"
#include "AdaptorCells/ParallelCell.h"

Parallel::Parallel()
{
    cell = std::make_unique<ParallelCell> (*this);
}

bool Parallel::prepare (double sampleRate)
{
    if (! TwoPort::prepare (sampleRate))
        return false;

    parallel = std::make_unique<chowdsp::WDF::WDFParallel<double>> (child[0]->getWDF(), child[1]->getWDF());
    wdf = parallel.get();

    return true;
}
