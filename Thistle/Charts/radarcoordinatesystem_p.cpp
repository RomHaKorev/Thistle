#include "radarcoordinatesystem_p.h"

namespace Thistle
{

RadarCoordinateSystemPrivate::RadarCoordinateSystemPrivate()
    : AbstractCoordinateSystemPrivate()
{
    centerHoleDiam = 0;
    origin = QPointF(0,0);
}

}