#include "pielegendview.h"
#include "piechart.h"
#include "private/pielegendview_p.h"
#include <QDebug>
#include <qabstractitemview.h>

namespace Thistle
{
PieLegendView::PieLegendView( QWidget* parent ) : AbstractLegendView( new PieLegendViewPrivate( this ), parent )
{}


PieLegendView::~PieLegendView()
{}

int PieLegendView::indexToSerie( const QModelIndex& index ) const
{
	return index.row();
}

QModelIndex PieLegendView::serieToIndex( int serie ) const
{
	if ( this->model() == 0 )
		return QModelIndex();

	return this->model()->index( serie, 0 );
}

void PieLegendView::paintSeriePicto( QPainter &painter, const QRect& rect, int serie ) const
{
	const Q_D( PieLegendView );
	QRect r( rect.x(), rect.y(), qMin( rect.width(), rect.height() ), qMin( rect.width(), rect.height() ) );
	painter.save();
	painter.setClipRect( rect );
	QColor color( this->model()->data( this->model()->index( serie, 0 ), Qt::DecorationRole ).toString() );
	if ( !color.isValid() )
		color = Colors::predefinedColor( serie );

	if ( Q_LIKELY( !d->associatedPieChart.isNull() ) )
	{
		d->associatedPieChart->configureColor( painter, color );
		QPen p = painter.pen();
		p.setWidth( 1 );
		painter.setPen( p );
	}
	else
	{
		painter.setPen( QPen( color.darker( 120), 1 ) );
		painter.setBrush( color );
	}
	painter.drawPie( r, -50 * 16, 80 * 16 );

	painter.setPen( Qt::lightGray );
	painter.setBrush( QColor( 200, 200, 200 ) );
	painter.drawPie( r.adjusted(1, 1, -1, -1), 40 * 16, 270 * 16 );

	painter.restore();
}


unsigned int PieLegendView::serieCount() const
{
	if ( this->model() != 0 )
		return this->model()->rowCount();
	else
		return 0;
}


QString PieLegendView::serieName( unsigned int serieIdx ) const
{
	return this->model()->headerData( serieIdx, Qt::Vertical ).toString();
}


void PieLegendView::setAssociatedPieChart( QPointer<PieChart> chart )
{
	Q_D( PieLegendView );
	d->associatedPieChart = chart;
}


QPointer<PieChart> PieLegendView::setAssociatedPieChart() const
{
	const Q_D( PieLegendView );
	return d->associatedPieChart;
}


QModelIndex PieLegendView::indexAt( const QPoint& point ) const
{
	const Q_D( PieLegendView );
	QPoint p = point + QPoint( horizontalOffset(), verticalOffset() );

	if ( this->model() == 0 )
		return QModelIndex();

	for ( unsigned int c = 0; c < this->serieCount(); ++c )
	{
		QRect r = d->serieRect( c );
		qDebug() << c << r;
		if ( r.contains( p ) )
		{
			qDebug() << Q_FUNC_INFO << this->model()->index( c, 0 );
			return this->model()->index( c, 0 );
		}
	}
	return QModelIndex();
}
}