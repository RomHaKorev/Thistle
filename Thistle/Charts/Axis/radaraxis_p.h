#ifndef RADARLAXIS_P_H
#define RADARLAXIS_P_H

#include "abstractaxis_p.h"

namespace Thistle
{

struct RadarAxisPrivate : public AbstractAxisPrivate
{
    qreal centerHoleDiam;
    QPointF origin;
    QLineF yaxis;

    RadarAxisPrivate();
};

}

#endif // RADARLAXIS_P_H
