#ifndef THISTLE_RADARLAXIS_P_H
#define THISTLE_RADARLAXIS_P_H

#include "abstractcoordinatesystem_p.h"

namespace Thistle
{

struct RadarCoordinateSystemPrivate : public AbstractCoordinateSystemPrivate
{
    qreal centerHoleDiam;
    QPointF origin;
    QLineF yaxis;

    RadarCoordinateSystemPrivate();
};

}

#endif // THISTLE_RADARLAXIS_P_H
