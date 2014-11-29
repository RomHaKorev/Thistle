#ifndef THISTLE_LINEARAXIS_P_H
#define THISTLE_LINEARAXIS_P_H

#include <QtCore>

namespace Thistle
{

struct LinearAxisPrivate
{
    qreal minBound;
    qreal maxBound;
    qreal min;
    qreal max;
    long order;
    int precision;
    int ticksCount;
    qreal tickSize;
    qreal labelsLength;
    QLineF line;
    LinearAxisPrivate();

    void calculateBounds();
};

}
#endif // THISTLE_LINEARAXIS_P_H