#include "piechart_p.h"

#include "../../base/abstractchart.h"
#include "../piechart.h"
#include <QFontMetrics>
#include <QDebug>

namespace Thistle
{

PieChartPrivate::PieChartPrivate( bool is3D, PieChart* q ) : AbstractChartPrivate( q )
{
	offsetFactor = 1.2;
	ring = false;
	splitted = false;
	startAngle = 0.0;
#if 0
	delete this->legend;
	this->legend = new PieChartLegend( q );
#endif
	this->is3D = is3D;
}


void PieChartPrivate::createRects( const QRect& area )
{
	double w = qMin( q_ptr->width(), q_ptr->height() ) * 0.80;

	this->rect.setWidth( w );
	this->rect.setHeight( w );

	QPointF p( double( q_ptr->width() - w ) / 2.0, double( q_ptr->height() - w ) / 2.0 );

	this->rect.moveTo( p.toPoint() );

	if ( this->is3D )
	{
		createRects3D( area );
	}
}

void PieChartPrivate::createRects3D( const QRect& area )
{
	Q_UNUSED( area )
	Q_Q( PieChart );
	if ( q->model() == 0 )
	{
		return;
	}

	if ( this->splitted == true )
	{
		this->rect.translate( 10, 10 );
		this->rect.setWidth( this->rect.width() - 20 );
		this->rect.setHeight( this->rect.height() - 20 );
	}
	this->height = this->rect.height() * 0.20;
	this->rect.setHeight( this->rect.height() * 0.75 );
	this->angles.clear();
	qreal angle = this->startAngle;
	for ( int i = 0; i < q->model()->rowCount(); ++i )
	{
		this->angles << angle;
		QModelIndex index = q->model()->index( i, 0 );
		qreal v = qAbs( q->model()->data( index ).toReal() );
		qreal delta = 360.0 * v/this->total;
		this->angles << delta;
		angle += delta;
	}
	this->angles << 360.0;
}

void PieChartPrivate::calculateLegendRect( const QRect& source )
{
	Q_Q( PieChart );
	QFontMetrics metrics( q->font() );
	//int h = metrics.height() + 10;
	int rows = q->model()->rowCount();

	int w = 0;

	for( int r = 0; r < rows; ++r )
	{
		QString s( q->model()->headerData( r, Qt::Vertical ).toString() );
		w = qMax( metrics.width( s ) + 40, w );
	}

	w = qMin( source.width(), w );
#if 0
	this->legend->area = QRect( source.right() - w, q->contentsMargins().top(), w, rows * h );
#endif
}

}
