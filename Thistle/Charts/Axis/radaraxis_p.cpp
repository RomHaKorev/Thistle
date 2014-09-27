#include "radaraxis_p.h"

namespace Thistle
{

RadarAxisPrivate::RadarAxisPrivate()
    : AbstractAxisPrivate()
{
    centerHoleDiam = 0;
    origin = QPointF(0,0);
    nbTicks = 5;
    nbDigits = 0;
}

}