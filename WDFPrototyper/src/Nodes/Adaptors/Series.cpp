#include "Series.h"
#include "AdaptorCells/SeriesCell.h"

Series::Series()
{
    cell = std::make_unique<SeriesCell> (*this);
}

bool Series::prepare (double sampleRate)
{
    if (! TwoPort::prepare (sampleRate))
        return false;

    series = std::make_unique<chowdsp::wdf::WDFSeries<double>> (child[0]->getWDF(), child[1]->getWDF());
    wdf = series.get();

    return true;
}
