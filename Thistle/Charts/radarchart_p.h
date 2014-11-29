#ifndef THISTLE_RADARCHART_P_H
#define THISTLE_RADARCHART_P_H

#include "linearchart_p.h"

namespace Thistle
{

class RadarCoordinateSystem;
class RadarChart;
class LinearChart;

struct RadarChartPrivate : public LinearChartPrivate
{
    RadarCoordinateSystem* radarAxis;
    RadarChartPrivate( RadarChart* q );
    ~RadarChartPrivate();
};

}

#endif // THISTLE_RADARCHART_P_H
