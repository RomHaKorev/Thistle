#ifndef THISTLE_KIVIATCHART_P_H
#define THISTLE_KIVIATCHART_P_H

#include "../../line/private/linechart_p.h"

namespace Thistle
{
class KiviatChart;

class KiviatChartPrivate: public LineChartPrivate
{
private:
	KiviatChart* q_func() const
	{
		return reinterpret_cast<KiviatChart*>( q_ptr );
	}
public:
	KiviatChartPrivate( KiviatChart* q, AbstractCoordinateSystemView* coordSysView );
	virtual ~KiviatChartPrivate();

	virtual void paintStraightLine( QPainter& painter, int column, bool isActive, bool area = false ) const;
	virtual void paintSpline( QPainter& painter, int column, bool isActive, bool area = false ) const;
};

}

#endif // THISTLE_KIVIATCHART_P_H
