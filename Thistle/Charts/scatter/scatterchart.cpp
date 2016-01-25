#include "scatterchart.h"
#include "private/scatterchart_p.h"

#include "../base/abstractcoordinatesystemview.h"
#include "../base/cartesiancoordinateplane.h"
#include "../base/serieformatproxy.h"

namespace Thistle
{
ScatterChart::ScatterChart( QWidget* parent ): LineChart( new ScatterChartPrivate( this ), parent )
{}


ScatterChart::~ScatterChart(void)
{}


QPainterPath ScatterChart::itemPath( const QModelIndex& index ) const
{
	const Q_D( ScatterChart );

	QPainterPath p;
	QRectF r;
	Thistle::Types t = this->serieFormat( index.column() ).type();

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

	p.addRect( r.normalized() );
	return p;
	//return r.normalized().toRect();
}


Thistle::SerieFormat ScatterChart::serieFormat( int column ) const
{
	const Q_D( ScatterChart );
	if ( d->formatProxy->contains( column ) )
		return d->formatProxy->serieFormat( column );

	Thistle::SerieFormat style;
	QColor c1 =  Colors::predefinedLightColor( column );
	QColor c2 = Colors::predefinedDarkColor( column );
	style.setPen( QPen( QColor(c2), 2 ) );
	style.setBrush( QBrush(c1) );
	style.setType( Thistle::Dot );
	return style;
}

}
