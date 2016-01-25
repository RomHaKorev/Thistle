#include "kiviatchart_p.h"

#include "../kiviatchart.h"

#include "../../base/abstractcoordinatesystemview.h"

namespace Thistle
{
KiviatChartPrivate::KiviatChartPrivate( KiviatChart* q, AbstractCoordinateSystemView* coordSysView )
	:LineChartPrivate( q, coordSysView )
{}


KiviatChartPrivate::~KiviatChartPrivate()
{}


void KiviatChartPrivate::paintStraightLine( QPainter& painter, int column, bool isActive, bool area ) const
{
	LineChartPrivate::paintStraightLine( painter, column, isActive, area );

	const Q_Q( KiviatChart );

	QColor penColor = painter.pen().color();

	QRectF r1 = q->itemRect( q->model()->index( 0, column ) );
	QRectF r2 = q->itemRect( q->model()->index( q->model()->rowCount() - 1, column ) );

	if ( !isActive )
	{
		QPointF p1 = r1.center();
		QPointF p2 = r2.center();
		QLineF line( p1, p2 );
		QLineF l = QLineF( line.pointAt( 0.5 ), line.p2() ).normalVector();
		l.setLength( 4 );
		QLinearGradient gradient( l.p1(), l.p2() );
		QColor c( penColor );
		c.setAlpha( 50 );
		gradient.setColorAt( 0, c );
		gradient.setColorAt( 1, Qt::transparent );
		gradient.setSpread( QLinearGradient::ReflectSpread );
		QPen pen( QBrush( gradient ), 8 );
		painter.setPen( pen );
	}
	painter.drawLine( r1.center(), r2.center() );
}

void KiviatChartPrivate::paintSpline( QPainter& painter, int column, bool isActive, bool area ) const
{
	if ( !isActive )
	{
		QPen pen = painter.pen();
		QColor c( pen.color() );
		c.setAlpha( c.alpha() * 0.5 );
		pen.setColor( c );
		painter.setPen( pen );

		QBrush brush = painter.brush();
		c = QColor( brush.color() );
		c.setAlpha( c.alpha() * 0.4 );
		brush.setColor( c );
		painter.setBrush( brush );
	}

	if ( !area )
		painter.setBrush( Qt::NoBrush );

	const Q_Q( SerieChart );
	int rows = q->model()->rowCount();

	if ( rows < 3 )
		return;

	QVector<QPointF> points;

	points << q->itemRect( rows - 1, column ).center();
	for ( int row = 0; row < rows; ++row )
		points << q->itemRect( row, column ).center();
	points << q->itemRect( 0, column ).center();

	QLineF l( points.at( 1 ), points.at( 0 ) );
	l.setLength( l.length() * 1.5 );
	points.prepend( l.p2() );

	l = QLineF( points.at( points.count() - 2 ), points.at( points.count() - 1 ) );
	l.setLength( l.length() * 1.5 );
	points << l.p2();

	QPair<QPointF, QPointF> pair = this->controlPoints( points.at( 0 ), points.at( 1 ), points.at( 2 ) );
	QPointF p0 = pair.second;

	QPainterPath path;
	path.moveTo( points.at( 1 ) );

	for ( int i = 2; i != points.count() - 2; ++i )
	{
		QPair<QPointF, QPointF> pair = this->controlPoints( points.at( i - 1 ), points.at( i ), points.at( i + 1 ) );
		path.cubicTo( p0, pair.first, points.at( i ) );
		p0 = pair.second;
	}

	if ( area )
	{
		path.moveTo( QPointF( points.at( points.count() - 3 ).x(), this->coordinateSysView->origin().y() ) );
		path.lineTo( QPointF( points.at( 1 ).x(), this->coordinateSysView->origin().y() ) );
		path.closeSubpath();
	}
	painter.drawPath( path );

}

}
