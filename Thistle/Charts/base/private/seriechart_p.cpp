#include "seriechart_p.h"

#include "../seriechart.h"
#include "../abstractcoordinatesystemview.h"
#include "../seriedelegates.h"

namespace Thistle
{
SerieChartPrivate::SerieChartPrivate( SerieChart* q, AbstractCoordinateSystemView* coordSysView )
	: AbstractChartPrivate( q ),
	  currentDelegate( 0 ),
	  coordinateSysView( coordSysView ),
	  pointDelegate( new DotDelegate( 0 ) ),
	  barDelegate( new BarDelegate( 0 ) )
{
	x = 0;
}

SerieChartPrivate::~SerieChartPrivate()
{
	this->barDelegate->deleteLater();
	this->pointDelegate->deleteLater();

	// Because we can get and delete these instances. We have to check beefore trying to destroy it.
	if ( this->coordinateSysView != 0 )
		delete this->coordinateSysView;

	this->coordinateSysView = 0;
}


AbstractCoordinateSystemView* SerieChartPrivate::coordinateSystemView() const
{
	return this->coordinateSysView;
}


void SerieChartPrivate::setCoordinateSystemView( AbstractCoordinateSystemView* cs )
{
	this->coordinateSysView = cs;
}

void SerieChartPrivate::readModel()
{
	/*int rows = this->model()->rowCount();
	int cols = this->model()->columnCount();
	QFontMetrics metrics( this->font() );
	int textWidth = 0;
	int valueWidth = 0;
	qreal value = this->model()->index( 0, 0 ).data().toDouble();
	qreal min = 0;
	qreal max = 0;
	for ( int r = 0; r < rows; ++r )
	{
		QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
		textWidth = qMax( textWidth, metrics.width( s ) + 5 );
		for ( int c = 0; c < cols; ++c )
		{
			value = this->model()->index( r, c ).data().toDouble();
			min = float( qMin( min, value ));
			max = float( qMax( max, value ));
			QString s = QString::number( value, 'f', this->precision(CartesianCoordinatePlane::Y) );
			valueWidth = qMax( valueWidth, metrics.width( s ) );
		}
	}

	//d->yAxis().setBounds( min, max );
	this->setBounds( CartesianCoordinatePlane::Y, min, max );
	this->setBounds( CartesianCoordinatePlane::X, 0, rows );
	this->setLabelsLength( CartesianCoordinatePlane::Y, valueWidth + 5 );
	this->setLabelsLength( CartesianCoordinatePlane::X, textWidth );*/
}

void SerieChartPrivate::selectDelegate( Thistle::Types type )
{
	if ( type.testFlag( Thistle::Dot ) )
		this->currentDelegate = this->pointDelegate;
	else if ( type.testFlag( Thistle::Bar ) )
		this->currentDelegate = this->barDelegate;
	else
		this->currentDelegate = 0;
}

void SerieChartPrivate::paintDelegate( QPainter& painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	if ( this->currentDelegate == 0 )
		return;
	this->currentDelegate->paint( &painter, option, index );
}

void SerieChartPrivate::paint( QPainter& painter, int column, Thistle::Types types, bool active ) const
{
	painter.save();

	if ( types.testFlag( Thistle::Line ) )
		this->paintStraightLine( painter, column, active, types.testFlag( Thistle::Area ) );
	else if ( types.testFlag( Thistle::Spline ) )
		this->paintSpline( painter, column, active, types.testFlag( Thistle::Area ) );

	if ( types.testFlag( Thistle::Dot ) || types.testFlag( Thistle::Bar ) )
		this->paintRaw( painter, column, active );

	painter.restore();
}

void SerieChartPrivate::paintRaw( QPainter& painter, int column, bool isActive ) const
{
	const Q_Q( SerieChart );
	int rows = q->model()->rowCount();
	for ( int r = 0; r < rows; ++r )
	{
		QModelIndex index = q->model()->index( r, column );
		QStyleOptionViewItem option;

		qreal value = index.data().toReal();

		if ( value < 0 )
			option.decorationPosition = QStyleOptionViewItem::Bottom;
		else
			option.decorationPosition = QStyleOptionViewItem::Top;

		if ( isActive == false )
			option.state = QStyle::State_Off;
		else
			option.state = QStyle::State_Selected;

		QRectF rect = q->itemRect( index );
		option.rect = rect.toRect();

		this->paintDelegate( painter, option, index );
	}
}

void SerieChartPrivate::paintStraightLine( QPainter& painter, int column, bool isActive, bool area ) const
{
	const Q_Q( SerieChart );
	int rows = q->model()->rowCount();
	QPolygonF polygon;

	QColor penColor = painter.pen().color();
	for ( int r = 0; r < rows; ++r )
	{
		QModelIndex index = q->model()->index( r, column );

		QRectF rect = q->itemRect( index );

		if ( r < ( rows - 1 ) )
		{
			QRectF r2 = q->itemRect( q->model()->index( r + 1, column ) );

			if ( !isActive )
			{
				QPointF p1 = rect.center();
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

			if ( area )
				polygon << rect.center();

			painter.drawLine( rect.center(), r2.center() );
		}
	}
	if ( area )
	{
		QPen pen = painter.pen();
		if ( !isActive )
		{
			QBrush brush = painter.brush();
			QColor c( brush.color() );
			c.setAlpha( 25 );
			brush.setColor( c );
			painter.setBrush( brush );
		}
		painter.setPen( Qt::NoPen );
		polygon << q->itemRect( q->model()->index( rows - 1, column ) ).center();
		polygon.prepend( QPointF( polygon.first().x(), this->coordinateSysView->origin().y() - 1 ) );
		polygon.append( QPointF( polygon.last().x(), this->coordinateSysView->origin().y() - 1 ) );
		polygon << q->itemRect( q->model()->index( rows - 1, column ) ).center();

		painter.drawPolygon( polygon );
		painter.setPen( pen );
		if ( isActive )
		{
			polygon.pop_front();
			polygon.pop_back();
			polygon.pop_back();
			painter.drawPolyline( polygon );
		}
	}
}


void SerieChartPrivate::paintSpline( QPainter& painter, int column, bool isActive, bool area ) const
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
	{
		painter.setBrush( Qt::NoBrush );
	}
	const Q_Q( SerieChart );
	int rows = q->model()->rowCount();
	if ( rows < 3 ) return;

	QVector<QPointF> points;

	for ( int row = 0; row < rows; ++row )
	{
		points << q->itemRect( row, column ).center();
	}

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

	for ( int i = 2; i != points.count() - 1; ++i )
	{
		QPair<QPointF, QPointF> pair = this->controlPoints( points.at( i - 1 ), points.at( i ), points.at( i + 1 ) );
		path.cubicTo( p0, pair.first, points.at( i ) );
		p0 = pair.second;
	}

	if ( area )
	{
		path.lineTo( QPointF( points.at( points.count() - 2 ).x(), this->coordinateSysView->origin().y() ) );
		path.lineTo( QPointF( points.at( 1 ).x(), this->coordinateSysView->origin().y() ) );
		path.closeSubpath();
	}
	painter.drawPath( path );
}


QPair<QPointF, QPointF> SerieChartPrivate::controlPoints( const QPointF& p0, const QPointF& p1, const QPointF& p2, qreal t ) const
{
	QPair<QPointF, QPointF> pair;
	qreal d01 = qSqrt( ( p1.x() - p0.x() ) * ( p1.x() - p0.x() ) + ( p1.y() - p0.y() ) * ( p1.y() - p0.y() ) );
	qreal d12 = qSqrt( ( p2.x() - p1.x() ) * ( p2.x() - p1.x() ) + ( p2.y() - p1.y() ) * ( p2.y() - p1.y() ) );

	qreal fa = t * d01 / ( d01 + d12 );
	qreal fb = t * d12 / ( d01 + d12 );

	qreal c1x = p1.x() - fa * ( p2.x() - p0.x() );
	qreal c1y = p1.y() - fa * ( p2.y() - p0.y() );
	qreal c2x = p1.x() + fb * ( p2.x() - p0.x() );
	qreal c2y = p1.y() + fb * ( p2.y() - p0.y() );

	pair.first = QPointF( c1x, c1y );
	pair.second = QPointF( c2x, c2y );

	return pair;
}

}
