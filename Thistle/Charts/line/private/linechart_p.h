#ifndef THISTLE_LINECHART_P_H
#define THISTLE_LINECHART_P_H

#include <QRect>
#include <QString>

#include "../../base/private/seriechart_p.h"

namespace Thistle
{
class AxisView;
class AbstractCoordinatePlaneView;

class LineChart;

class LineChartPrivate : public SerieChartPrivate
{
protected:
    LineChart* q_func() const
	{
        return reinterpret_cast<LineChart*>(q_ptr);
	}

public:
    LineChartPrivate( LineChart* q );
    LineChartPrivate( LineChart* q, AbstractCoordinateSystemView* coordView );
    virtual ~LineChartPrivate();

	void updateAxis();
};

}

#endif // THISTLE_LINECHART_P_H
