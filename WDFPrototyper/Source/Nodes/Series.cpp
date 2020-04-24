#include "Series.h"
#include "../GUI/SeriesCell.h"

Series::Series()
{
    cell = std::make_unique<SeriesCell> (*this);
}

bool Series::prepare (double sampleRate)
{
    bool result = TwoPort::prepare (sampleRate);

    if (result)
        wdf = std::make_unique<WaveDigitalFilter::WDFSeries> (child[0]->getWDF(), child[1]->getWDF());

    return result;
}
