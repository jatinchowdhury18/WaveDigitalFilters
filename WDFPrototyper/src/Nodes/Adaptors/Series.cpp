#include "Series.h"
#include "AdaptorCells/SeriesCell.h"

Series::Series()
{
    cell = std::make_unique<SeriesCell> (*this);
}

bool Series::prepare (double sampleRate)
{
    bool result = TwoPort::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<chowdsp::WDF::WDFSeries<double>> (child[0]->getWDF(), child[1]->getWDF());

    return result;
}
