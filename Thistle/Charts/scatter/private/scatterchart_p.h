#ifndef THISTLE_SCATTERCHART_P_H
#define THISTLE_SCATTERCHART_P_H

#include "../../line/private/linechart_p.h"
#include "../../base/abstractcoordinatesystem.h"

#include <QMap>

namespace Thistle
{
class ScatterChart;

struct ScatterChartPrivate : public LineChartPrivate
{
	ScatterChart* q_func() const
	{
		return reinterpret_cast<ScatterChart*>( q_ptr );
	}

	ScatterChartPrivate( ScatterChart* q );
	virtual void paintRaw( QPainter& painter, int column, bool isActive ) const;
};

}

#endif // THISTLE_SCATTERCHART_P_H
