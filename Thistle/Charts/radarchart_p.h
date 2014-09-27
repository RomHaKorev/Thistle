#ifndef RADARCHART_P_H
#define RADARCHART_P_H

#include "linearchart_p.h"

namespace Thistle
{

class RadarAxis;
class RadarChart;
class LinearChart;

struct RadarChartPrivate : public LinearChartPrivate
{
    RadarAxis* radarAxis;
    RadarChartPrivate( RadarChart* q );
    ~RadarChartPrivate();
};

}

#endif // RADARCHART_P_H
