#ifndef THISTLE_LINEARCHART_P_H
#define THISTLE_LINEARCHART_P_H

#include <QRect>
#include <QString>

#include "../../base/private/seriechart_p.h"

namespace Thistle
{
class AxisView;
class AbstractCoordinatePlaneView;

class LinearChart;

class LinearChartPrivate : public SerieChartPrivate
{
protected:
    LinearChart* q_func() const
    {
        return reinterpret_cast<LinearChart*>(q_ptr);
    }

public:
    LinearChartPrivate( LinearChart* q );
	LinearChartPrivate( LinearChart* q, AbstractCoordinateSystemView* coordView );
    virtual ~LinearChartPrivate();
   
    void updateAxis();
};

}

#endif // THISTLE_LINEARCHART_P_H
