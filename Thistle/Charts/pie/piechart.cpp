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
#include "../../Core/global.h"
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
		path = this->itemPart( angle, delta, true );
	else
		path = this->itemPart( angle, delta );
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
	painter.save();
	painter.setRenderHint( QPainter::Antialiasing );

	int rows = this->model()->rowCount();
	for ( int i = 0; i < rows; ++i )
	{
		QModelIndex index = this->model()->index( i, 0 );
		QPainterPath path = this->itemPath( index );
		QColor color( this->model()->data( index, Qt::DecorationRole ).toString() );
		if ( !color.isValid() )
			color = Colors::predefinedColor( i );
		int flag = 0;
		this->configureColor( painter, color, flag );
		painter.setClipPath( path );
		painter.drawPath( path );
	}
	painter.restore();
}


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
	QPainterPath part = this->itemPart( angle, delta, true );
	painter.save();
	if ( ( !this->selectionModel()->selectedIndexes().isEmpty() || this->currentIndex().isValid() )
		 && isSelected == false )
		this->configureColor( painter, color, 2 );
	else
		this->configureColor( painter, color, 1 );
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
