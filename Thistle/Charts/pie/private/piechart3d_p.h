#ifndef THISTLE_PIECHART3D_P_H
#define THISTLE_PIECHART3D_P_H

#include <QRect>
#include <QString>
#include <QList>
#include <QDebug>
#include <QPainterPath>
#include "piechart_p.h"
//#include "piechart3D.h"

namespace Thistle
{

class PieChart;

class PieChart3DPrivate : public PieChartPrivate
{
public:
	PieChart3DPrivate( PieChart* q );

	virtual void createRects( const QRect& area );

};

}
#endif // THISTLE_PIECHART3D_P_H
