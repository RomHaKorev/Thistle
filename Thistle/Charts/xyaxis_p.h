#ifndef THISTLE_XYAXIS_P_H
#define THISTLE_XYAXIS_P_H

#include "orthogonalaxis_p.h"

namespace Thistle
{

struct XYAxisPrivate : public OrthogonalAxisPrivate
{
    XYAxisPrivate();

    qreal xMinBound;
    qreal xMaxBound;
    qreal xMin;
    qreal xMax;
    long xOrder;
    int xNbDigits;
    int xNbTicks;
};

}

#endif // THISTLE_XYAXIS_P_H
