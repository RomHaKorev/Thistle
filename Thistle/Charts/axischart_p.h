#ifndef THISTLE_AXISCHART_P_H
#define THISTLE_AXISCHART_P_H

#include "abstractchart_p.h"
#include "Axis/abstractcoordinatesystem.h"

#include <QMap>

namespace Thistle
{
class AxisChart;
class AxisView;

struct AxisChartPrivate : public AbstractChartPrivate
{
    AxisView* axisView;
    int x;

    AxisChartPrivate( AxisChart* q );

    void readModel();
};

}

#endif // THISTLE_AXISCHART_P_H
