#ifndef THISTLE_PIECHART_P_H
#define THISTLE_PIECHART_P_H

#include <QRect>
#include <QString>
#include <QList>

#include "../../base/private/abstractchart_p.h"

namespace Thistle
{

class PieChart;

class PieChartPrivate : public AbstractChartPrivate
{
public:
	PieChart* q_func()
	{
		return reinterpret_cast<PieChart*>(q_ptr);
	}

	qreal offsetFactor;
	bool is3D;
	QList<double> angles;
	QRect rect;
	qreal total;
	bool splitted;
	bool ring;
	qreal startAngle;
	int render;
	qreal height;

	QPainterPath front;
	PieChartPrivate( bool is3d, PieChart* q );

	void createRects( const QRect& area );
	void createRects3D( const QRect& area );
	void calculateLegendRect( const QRect& source );
};

}

#endif // THISTLE_PIECHART_P_H
