#ifndef THISTLE_RADIALCHART_P_H
#define THISTLE_RADIALCHART_P_H

#include <QRect>
#include <QString>

#include "axischart_p.h"

namespace Thistle
{

class RadialCoordinateSystem;
class RadialChart;
class AxisChart;

struct RadialChartPrivate : public AxisChartPrivate
{
    qreal centerHoleDiam;
    RadialCoordinateSystem* radialAxis;
    RadialChartPrivate( RadialChart* q );
    ~RadialChartPrivate();
};

}

#endif // THISTLE_RADIALCHART_P_H
