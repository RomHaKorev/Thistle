#include "pointchart.h"
#include "private/pointchart_p.h"

#include "../base/abstractcoordinatesystemview.h"
#include "../base/cartesiancoordinateplane.h"

namespace Thistle
{
PointChart::PointChart( QWidget* parent ): LinearChart( new PointChartPrivate( this ), parent )
{}


PointChart::~PointChart(void)
{}


QRectF PointChart::itemRect( const QModelIndex& index ) const
{
    const Q_D( PointChart );

    QRectF r;
    Thistle::Types t = this->columnType( index.column() );

    QPointF value = index.data().toPointF();

	CartesianCoordinatePlane* coordSys = dynamic_cast< CartesianCoordinatePlane* >( d->coordinateSystemView()->coordinateSystem() );

    QPointF pos = coordSys->valueToPoint( value.x(), value.y() );
	if ( t == Thistle::Bar )
	{
		qreal w = 14;
		pos += QPointF( -7, 0 );

		QPointF br( pos.x() + w, d->coordinateSystemView()->origin().y() );
		r = QRectF( pos, br );
		if ( value.y() < 0 )
		{
			r.translate( 0, 1 );
		}
		else
		{
			r.translate( 0, -1 );
		}
	}
	else
	{
		r = QRectF( -7, -7, 14, 14 ).translated( pos.x(), pos.y() );
	}
	return r.normalized().toRect();
    r = QRectF( -7, -7, 14 ,14 ).translated( pos.x(), pos.y() );
    
    return r.normalized().toRect();
}

}
