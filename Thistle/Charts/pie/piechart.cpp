/*
This file is part of Thistle.
Thistle is free software: you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.
Thistle is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
Lesser GNU General Public License for more details.
You should have received a copy of the Lesser GNU General Public License
along with Thistle.    If not, see <http://www.gnu.org/licenses/>.
Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/


#include "piechart.h"
#include "../../kernel/global.h"
#include "../base/abstractchart.h"

#include <QDebug>

#include <QPainter>
#include <QPaintEvent>

namespace Thistle
{

PieChart::PieChart( QWidget* parent ) :
	AbstractChart( new PieChartPrivate( false, this ), parent )
{
	this->setEditTriggers( QAbstractItemView::NoEditTriggers );
}

PieChart::PieChart( PieChartPrivate* d, QWidget* parent ) : AbstractChart( d, parent )
{
	this->setEditTriggers( QAbstractItemView::NoEditTriggers );
}


qreal PieChart::startAngle() const
{
	const Q_D( PieChart );
	return d->startAngle;
}


void PieChart::configureColor(QPainter &painter, QColor base, int flag ) const
{
	switch ( flag )
	{
		case 0:
		default:
			painter.setPen( QPen( base.darker( 105 ), 4 ) );
			painter.setBrush( base );
		break;
		case 1:
			painter.setPen( QPen( base.darker( 105 ), 2 ) );
			painter.setBrush( base );
		break;
		case 2:
			base.setAlpha( base.alpha() * 0.75 );
			painter.setPen( QPen( base.lighter( 110 ), 2 ) );
			painter.setBrush( base.lighter( 120 ) );
	}
}


QPainterPath PieChart::itemPart( qreal angle, qreal delta, bool splitted ) const
{
	const Q_D( PieChart );
	QPainterPath part;
	part.moveTo( d->rect.center() );
	part.arcTo( d->rect, -angle, -delta );
	if ( splitted == true )
	{
		QPointF p = this->splittedOffset( angle, delta );
		part.translate( p.x(), p.y() );
	}
	part.closeSubpath();
	if ( d->ring == true )
	{
		QPainterPath p;
		p.addEllipse( d->rect.center(), d->rect.width() * 0.2, d->rect.height() * 0.2 );
		part = part.subtracted( p );
	}
	return part;
}


QPainterPath PieChart::itemPath( const QModelIndex& index ) const
{
	const Q_D( PieChart );
	QPainterPath path;
	qreal angle = d->startAngle;
	for ( int r = 0; r < index.row(); ++r )
	{
		QModelIndex id = this->model()->index( r, 0 );
		qreal v = qAbs( this->model()->data( id ).toReal() );
		qreal delta = 360.0 * v/d->total;
		angle += delta;
	}
	qreal v = qAbs( this->model()->data( index ).toReal() );
	qreal delta = 360.0 * v/d->total;
	if ( this->selectionModel()->selectedIndexes().contains( index ))
	{
		path = this->itemPart( angle, delta, true );
	}
	else
	{
		path = this->itemPart( angle, delta );
	}
	return path;
}


void PieChart::paintEvent( QPaintEvent* ev )
{
	Q_UNUSED( ev );

	QPainter painter( this->viewport() );

	paint( painter );
}


void PieChart::paint( QPainter& painter )
{
	const Q_D( PieChart );

	painter.save();
	painter.setRenderHint( QPainter::Antialiasing );

	int rows = this->model()->rowCount();
	qreal angle = d->startAngle;
	for ( int i = 0; i < rows; ++i )
	{
		QModelIndex index = this->model()->index( i, 0 );
		QColor color( this->model()->data( index, Qt::DecorationRole ).toString() );
		if ( !color.isValid() )
		{
			color = Colors::predefinedColor( i );
		}
		qreal v = qAbs( this->model()->data( index ).toReal() );
		qreal delta = 360.0 * v/d->total;

		bool isSelected = this->selectionModel()->selectedIndexes().contains( index );

		if ( d->splitted == false )
		{
			this->paintPart( painter, angle, delta, color, isSelected );
		}
		else
		{
			this->paintPartSplitted( painter, angle, delta, color, isSelected );
		}
		angle += delta;
	}
	painter.restore();
}


#if 0
/*Paint the legend in the QRect self._legendRect
The legend corresponds to the text in the Horizontal QHeaderView and the style defined for each column.
*/
void PieChart::paintLegend( QPainter& painter) const
{
	return;
	const Q_D( PieChart );
	painter.save();
	QFontMetrics metrics( this->font() );
	int metricsH = metrics.height();
	int h = metricsH + 10;
	int rows = this->model()->rowCount();
	int w = 0;
	int maxWidth = d->legend->area.width();
	QPoint legendPos( d->legend->area.topLeft() );
	QPoint pos = legendPos + QPoint( 50, 0);
	for (int r = 0; r < rows; ++r )
	{
		QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
		int sWidth = metrics.width( s ) + 50;
		QPoint p;
		if ( ( w + sWidth ) > maxWidth )
		{
			int y = pos.y();
			p = QPoint( d->legend->area.x(), y + h );
			pos = QPoint( d->legend->area.x(), y + h );
			w = sWidth;
			pos += QPoint( sWidth, 0 );
		}
		else
		{
			p = pos + QPoint( -40,    0 );
			w += sWidth;
			pos += QPoint( sWidth, 0 );
		}
		this->paintSerieLegend(painter, r, p, metricsH);
	}
	painter.restore();
}


void PieChart::paintSerieLegend( QPainter& painter, int serie, QPoint pos, int metricsH ) const
{

	QColor color( this->model()->data( this->model()->index( serie, 0 ), Qt::DecorationRole ).toString() );
	if ( !color.isValid() )
	{
		color = Colors::predefinedColor( serie );
	}

	QRect r( pos.x(), pos.y() - 20, 14, 14 );
	QPoint posText = pos + QPoint( 20, -metricsH/2 );

	QString s = this->model()->headerData( serie, Qt::Vertical ).toString();
	painter.drawText( posText, s );
	painter.save();
	painter.setBrush( color );
	painter.setPen( QPen( color.darker( 100 ), 1 ) );
	painter.drawRect( r );

	painter.restore();
}
#endif


void PieChart::paintPart( QPainter& painter, qreal angle, qreal delta, QColor color, bool isSelected ) const
{
	if ( isSelected == true )
	{
		this->paintPartSplitted( painter, angle, delta, color );
		return;
	}
	QPainterPath part = this->itemPart( angle, delta );
	painter.save();
	painter.setClipPath( part ); /* To avoid the "borders superposition" */
	int flag = 0;
	this->configureColor( painter, color, flag );
	painter.drawPath( part );
	painter.restore();
}


void PieChart::paintPartSplitted( QPainter &painter, qreal angle, qreal delta,
								  QColor color, bool isSelected ) const
{
	const Q_D( PieChart );
	QPainterPath part = this->itemPart( angle, delta, true );
	painter.save();
	if ( d->splitted == true
		 && ( !this->selectionModel()->selectedIndexes().isEmpty() || this->currentIndex().isValid() )
		 && isSelected == false )
	{
		this->configureColor( painter, color, 2 );
	}
	else
	{
		this->configureColor( painter, color, 1 );
	}
	painter.drawPath( part );
	painter.restore();
}


void PieChart::scrollTo(const QModelIndex &/*index*/, ScrollHint /*hint*/)
{
}


void PieChart::setRing( bool ring )
{
	Q_D( PieChart );
	d->ring = ring;
}


void PieChart::setSplitted( bool splitted )
{
	Q_D( PieChart );
	d->splitted = splitted;
}


void PieChart::setStartAngle( qreal angle )
{
	Q_D( PieChart );
	d->startAngle = angle;
	this->viewport()->update();
}


QPointF PieChart::splittedOffset( qreal angle, qreal delta ) const
{
	const Q_D( PieChart );
	QPainterPath part;
	part.moveTo( d->rect.center() );
	part.arcTo( d->rect, -angle, -delta );
	part.closeSubpath();
	QPointF p = part.pointAtPercent( 0.5 );
	QLineF line( p, d->rect.center() );
	line.setLength( line.length() * d->offsetFactor );
	p = line.p2();
	return d->rect.center() - p;
}


void PieChart::updateRects()
{
	Q_D( PieChart );
	if ( this->model() == 0 )
		return;

	d->createRects( this->contentsRect() );
}


QRect PieChart::visualRect( const QModelIndex& index ) const
{
	Q_UNUSED(index)
	/* To force to repaint the whole chart. Not only the area of the part*/
	return QRect( 0, 0, width(), height() );
}

void PieChart::scan()
{
	Q_D( PieChart );

	int rows = this->model()->rowCount();

	d->total = 0;
	for ( int r = 0; r < rows; ++r )
	{
		QVariant var = this->model()->index( r, 0 ).data();
		qreal value = var.toDouble();
		d->total += value;
	}
}


}
