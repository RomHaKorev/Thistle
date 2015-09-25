#ifndef THISTLE_SCATTERCHART_P_H
#define THISTLE_SCATTERCHART_P_H

#include "../../linear/private/linearchart_p.h"
#include "../../base/abstractcoordinatesystem.h"

#include <QMap>

namespace Thistle
{
class ScatterChart;

struct ScatterChartPrivate : public LinearChartPrivate
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
