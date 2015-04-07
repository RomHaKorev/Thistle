#ifndef THISTLE_POINTCHART_P_H
#define THISTLE_POINTCHART_P_H

#include "../../linear/private/linearchart_p.h"
#include "../../base/abstractcoordinatesystem.h"

#include <QMap>

namespace Thistle
{
class PointChart;
struct PointChartPrivate : public LinearChartPrivate
{
	PointChart* q_func() const
	{
		return reinterpret_cast<PointChart*>( q_ptr );
	}

    PointChartPrivate( PointChart* q );
	virtual void paintRaw( QPainter& painter, int column, bool isActive ) const;
};

}

#endif // THISTLE_POINTCHART_P_H
