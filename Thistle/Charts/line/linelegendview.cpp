#include "linelegendview.h"
#include "private/linelegendview_p.h"
#include "linechartwidget.h"
#include "linechart.h"
#include "../base/seriedelegates.h"
#include "../base/serieformat.h"

#include <QPainter>
#include <QWidget>
#include <QScrollBar>

namespace Thistle
{
LineLegendView::LineLegendView( QWidget* parent ) : AbstractLegendView( new LineLegendViewPrivate( this ), parent )
{
	Q_D( LineLegendView );
	//d->dotDelegate->serieFormatProxy()->deleteLater();
}


LineLegendView::~LineLegendView()
{}

int LineLegendView::indexToSerie( const QModelIndex& index ) const
{
	return index.column();
}

QModelIndex LineLegendView::serieToIndex( int serie ) const
{
	if ( this->model() == 0 )
		return QModelIndex();

	return this->model()->index( 0, serie );
}

void LineLegendView::paintSeriePicto( QPainter& painter, const QRect& rect, int serie ) const
{
	const Q_D( LineLegendView );
	QPoint p1( rect.left(), rect.center().y() );
	QPoint p2( rect.right(), rect.center().y() );
	SerieFormat style = d->formatProxy->serieFormat( serie );
	Thistle::Types t = style.type();

	bool isActive = this->isActiveColumn( serie );

	painter.save();

	QPen pen( style.pen() );
	pen.setWidth( 2 );

	if ( !isActive )
	{
		QColor c( pen.color() );
		c.setAlpha( c.alpha() * 0.5 );
		pen.setColor( c );
	}

	painter.setPen( pen );
	painter.setBrush( style.brush() );

	if ( t.testFlag( Thistle::Area ) )
	{
		painter.setBrush( style.brush() );
		QPolygon poly;
		poly << p1 + QPoint( 0, 8 ) << p1 << p1 + QPoint( 10, -8 ) << p1 + QPoint( 15, -3 ) << p1 + QPoint( 20, -6 ) << p2 << p2 + QPoint( 0, 8 );
		painter.drawPolygon( poly );
	}
	else if ( t.testFlag( Thistle::Line ) )
	{
		painter.drawLine( p1, p2 );
	}
	else if ( t.testFlag( Thistle::Spline ) )
	{
		QBrush brush = painter.brush();
		painter.setBrush( Qt::NoBrush );
		QPainterPath path;
		QPoint p11 = p1 + QPoint( 0, 7 );
		QPoint p22 = p2 - QPoint( 0, 7 );
		path.moveTo( p1 );
		path.cubicTo( p11  + QPoint( 25, 0 ), p22 - QPoint( 25, 0 ), p22 );
		painter.drawPath( path );
		painter.setBrush( brush );
	}
	else if ( t.testFlag( Thistle::Bar ) )
	{
		int j = 0;
		Q_FOREACH( int i, QList<int>() << 15 << 8 << 17 << 5 )
		{
			painter.setPen( Qt::NoPen );
			QRect r( p1.x() + j * 8, rect.bottom() - i, 5, i );
			painter.drawRect( r );
			j += 1;
		}
	}

	if ( t.testFlag( Thistle::Dot ) )
	{
		QStyleOptionViewItem option;
		option.rect = QRect( p1.x() + abs(p1.x() - p2.x())/2 - 5, p1.y() - 5, 10, 10 );
		option.state = isActive ? QStyle::State_On : QStyle::State_Off;
		if ( d->dotDelegate->serieFormatProxy().isNull() )
			d->dotDelegate->setSerieFormatProxy( this->serieFormatProxy() );
		d->dotDelegate->paint( &painter, option, this->model()->index( 0, serie ) );
	}
	painter.restore();
}


bool LineLegendView::isActiveColumn( int column ) const
{
	bool isActive = true;
	if ( this->selectionModel() != 0 )
	{
		QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
		if ( !selectedIndexes.isEmpty() )
		{
			isActive = false;
			Q_FOREACH( QModelIndex idx, selectedIndexes )
			{
				if ( idx.column() == column )
				{
					isActive = true;
					break;
				}
			}
		}
	}
	return isActive;
}


unsigned int LineLegendView::serieCount() const
{
	if ( this->model() != 0 )
		return this->model()->columnCount();
	return 0;
}

QString LineLegendView::serieName( unsigned int serieIdx ) const
{
	return this->model()->headerData( serieIdx, Qt::Horizontal ).toString();
}

QModelIndex LineLegendView::indexAt( const QPoint& point ) const
{
	const Q_D( LineLegendView );
	QPoint p = point + QPoint( horizontalOffset(), verticalOffset() );

	if ( this->model() == 0 )
		return QModelIndex();

	for ( unsigned int c = 0; c < this->serieCount(); ++c )
	{
		QRect r = d->serieRect( c );
		if ( r.contains( p ) )
			return this->model()->index( 0, c );
	}
	return QModelIndex();
}


}
