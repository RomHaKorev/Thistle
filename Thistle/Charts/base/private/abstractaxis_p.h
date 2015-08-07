#ifndef THISTLE_ABSTRACTAXIS_P_H
#define THISTLE_ABSTRACTAXIS_P_H

#include "../abstractaxis.h"

#include <QtCore>

namespace Thistle
{
struct AbstractAxisPrivate
{
    qreal minBound;
    qreal maxBound;
    qreal min;
    qreal max;
    double order;
    int precision;
    int ticksCount;
    qreal tickIncrement;
    qreal labelsLength;
    QLineF line;
	QString name;
    AbstractAxisPrivate();

    void calculateBounds();
};

}
#endif // THISTLE_ABSTRACTAXIS_P_H