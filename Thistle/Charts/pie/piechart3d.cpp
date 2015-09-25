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

#include "piechart3d.h"
#include "../../kernel/global.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

namespace Thistle
{


PieChart3D::PieChart3D( QWidget* parent ) :
	PieChart( parent )
{
	Q_D( PieChart );
	//d->splitted = true;
	d->is3D = true;
	d->render = Translucent;
	//myRender = Plain;
}


void PieChart3D::configureColor(QPainter &painter, QColor base, int flag) const
{
	const Q_D( PieChart );
	painter.setPen( QPen( base, 2 ) );
	switch( flag )
	{
		case 2:
			base = base.darker( 110 );
		break;
		case 3:
			base = base.darker( 120 );
		default:
		break;
	}
	switch( d->render )
	{
		case PieChart3D::Plain:
			painter.setPen( QPen( base.darker( 110 ), 2 ) );
		break;
		case PieChart3D::Translucent:
			base.setAlpha( 125 );
		break;
		case PieChart3D::WireFrame:
			base.setAlpha( 10 );
		break;
	}
	painter.setBrush( base );
}


QPainterPath PieChart3D::itemExternalPart( qreal angle, qreal delta, bool splitted ) const
{
	const Q_D( PieChart );
	QPainterPath ell;
	ell.addEllipse( d->rect );
	qreal a = angle / 360.0;
	if ( a > 1 )
	{
		a -= int(a);
	}
	QPointF p1 = ell.pointAtPercent( a );
	QPointF offset = QPointF( 0, d->height );
	QPainterPath outside;
	outside.setFillRule( Qt::WindingFill );
	outside.moveTo( p1 );
	outside.arcTo( d->rect, -angle, -delta );
	outside.lineTo( outside.currentPosition() + offset );
	outside.arcTo( d->rect.translated( 0, d->height ), -angle - delta, delta );
	outside.lineTo( p1 );
	if ( splitted == true )
	{
		QPointF p = splittedOffset( angle, delta );
		outside.translate( p.x(), p.y() );
	}
	return outside;
}


QPainterPath PieChart3D::itemSidesPath( const QModelIndex& index ) const
{
	const Q_D( PieChart );
	QPainterPath outside;
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
	if ( selectionModel()->selectedIndexes().contains( index ) )
	{
		outside = itemExternalPart( angle, delta, true );
	}
	else
	{
		outside = itemExternalPart( angle, delta );
	}
	return outside;
}


void PieChart3D::paint( QPainter& painter )
{
	const Q_D( PieChart );
	painter.save();
	painter.setRenderHint( QPainter::Antialiasing );
	if ( d->render != Plain )
	{
		paintExternal( painter, true );
		paintSides( painter );
		paintExternal( painter, false );
	}
	else
	{
		this->paintExternal( painter, false );
	}
	for ( int i = 0; i < d->angles.count() - 2; i += 2 )
	{
		QModelIndex index = this->model()->index( i/2, 0 );
		QColor color( this->model()->data( index, Qt::DecorationRole ).toString() );
		if ( !color.isValid() )
		{
			color = Colors::predefinedColor( i/2 );
		}

		bool isSelected = this->selectionModel()->selectedIndexes().contains( index );
		if ( d->splitted == false )
		{
			paintPart( painter, d->angles[i], d->angles[i + 1], color, isSelected );
		}
		else
		{
			paintPartSplitted( painter, d->angles[i], d->angles[i + 1], color, isSelected );
		}
	}

	painter.restore();
}


void PieChart3D::paintExternal( QPainter& painter, bool top )
{
	Q_D( PieChart );
	for ( int i = 0; i < d->angles.count() - 2; i+=2 )
	{
		QModelIndex index = this->model()->index( i/2, 0 );
		bool isSelected = this->selectionModel()->selectedIndexes().contains( index );
		QColor color( this->model()->data( index, Qt::DecorationRole ).toString() );
		if ( !color.isValid() )
		{
			color = Colors::predefinedColor( i/2 );
		}
		painter.save();
		qreal a1 = d->angles[i];
		qreal delta = d->angles[i + 1];
		qreal a2 = a1 + delta;
		if ( a1 < 180 && a2 > 180 )
		{
			QPointF  offset( 0, 0 );
			if ( isSelected ) offset = splittedOffset( d->angles[i], delta );
			this->paintLeft( painter, color, offset );
		}
		if ( top == false )
		{
			if ( a1 <= 180 && a2 > 180 )
			{
				if ( d->render == Plain )
				{
					configureColor( painter, color, 1 );
					qreal delta = 180.0 - a1;
					QPointF offset = splittedOffset( d->angles[i], delta );
					QPainterPath path = itemExternalPart( d->angles[i], delta, d->splitted );
					painter.drawPath( path );
					painter.setPen( Qt::NoPen );
					path = side( 180, offset, isSelected );
					painter.drawPath( path );
				}
				painter.restore();
				continue;
			}
			if ( a1 > 180 || a2 > 180 )
			{
				painter.restore();
				continue;
			}
		}
		else
		{
			if ( a1 < 180 && a2 < 180 )
			{
				painter.restore();
				continue;
			}
		}
		QPainterPath path = itemExternalPart( a1, delta, isSelected );
		configureColor( painter, color, 3 );
		painter.drawPath( path );
		painter.restore();
	}
}


void PieChart3D::paintLeft( QPainter& painter, QColor color, QPointF offset )
{
	Q_D( PieChart );
	if ( d->render == WireFrame )
	{
		return;
	}
	painter.save();
	color.setAlpha( 125 );
	configureColor( painter, color, 0 );
	int width = painter.pen().width()/2;
	painter.setPen( Qt::NoPen );
	QPainterPath path;
	QPainterPath ellipse1, ellipse2;
	ellipse1.addEllipse( d->rect );
	ellipse2.addEllipse( d->rect.translated( 0, d->height ) );
	path.moveTo( ellipse1.pointAtPercent( 0.5 ) + QPointF( -width, 0 ) );
	path.lineTo( ellipse2.pointAtPercent( 0.5 ) + QPointF( -width, 0 ) );
	path.arcTo( d->rect.translated( 0, d->height ), 180, -90 );
	path.moveTo( ellipse1.pointAtPercent( 0.5 ) );
	path.arcTo( d->rect, 180, 90 );
	path = path.subtracted( ellipse1 );
	path.translate( offset );
	painter.fillPath( path, color );
	painter.restore();
}


void PieChart3D::paintSides( QPainter& painter )
{
	Q_D( PieChart );
	QList<QPair<QPainterPath, QColor> > rightBottom;
	QList<QPair<QPainterPath, QColor> > rightTop;
	QList<QPair<QPainterPath, QColor> > leftBottom;
	QList<QPair<QPainterPath, QColor> > leftTop;
	for ( int i = 0; i < d->angles.count() - 2; i+=2 )
	{
		QModelIndex index = this->model()->index( i/2, 0 );
		bool isSelected = this->selectionModel()->selectedIndexes().contains( index );
		QColor color( this->model()->data( index, Qt::DecorationRole ).toString() );
		if ( !color.isValid() )
		{
			color = Colors::predefinedColor( i/2 );
		}
		QPointF offset = splittedOffset( d->angles[i], d->angles[i + 1] );
		QPainterPath path = this->side( d->angles[i], offset, isSelected );
		if ( d->angles[i] <= 90 )
		{
			rightBottom << QPair<QPainterPath, QColor>(path, color);
		}
		else if ( d->angles[i] <= 180 )
		{
			leftBottom.prepend( QPair<QPainterPath, QColor>(path, color) );
		}
		else if ( d->angles[i] <= 270 )
		{
			leftTop.prepend( QPair<QPainterPath, QColor>(path, color) );
		}
		else
		{
			rightTop << QPair<QPainterPath, QColor>(path, color);
		}
		path = side( d->angles[i] + d->angles[i+1], offset, isSelected );
		if ( d->angles[i] <= 90 )
		{
			rightBottom << QPair<QPainterPath, QColor>(path, color);
		}
		else if ( d->angles[i] <= 180 )
		{
			leftBottom.prepend( QPair<QPainterPath, QColor>(path, color) );
		}
		else if ( d->angles[i] <= 270 )
		{
			leftTop.prepend( QPair<QPainterPath, QColor>(path, color) );
		}
		else
		{
			rightTop << QPair<QPainterPath, QColor>(path, color);
		}
	}
	QPair<QPainterPath, QColor> pair;
	leftTop << rightTop << leftBottom << rightBottom;
	Q_FOREACH( pair, leftTop )
	{
		configureColor( painter, pair.second, 2 );
		painter.drawPath( pair.first );
	}
}


void PieChart3D::setRender( PieChart3D::Render r )
{
	Q_D( PieChart );
	if ( r == PieChart3D::Plain )
	{
		qWarning( "Not implemented yet." );
		return;
	}
	d->render = r;
}


QPainterPath PieChart3D::side( qreal angle, QPointF centerOffset, bool splitted ) const
{
	const Q_D( PieChart );
	QPainterPath ell;
	QRectF r = d->rect;
	if ( splitted == true )
	{
		r.translate( centerOffset.x(), centerOffset.y() );
	}
	ell.addEllipse( r );
	qreal a = angle / 360.0;
	if ( a > 1 )
	{
		a -= int(a);
	}
	QPointF p1 = ell.pointAtPercent( a );
	QPointF center = r.center();
	QPointF offset = QPointF( 0, d->height );
	QPainterPath outside;
	outside.setFillRule( Qt::WindingFill );
	QPolygonF sides;
	sides << center << p1 << p1 + offset << center + offset << center;
	outside.moveTo( center );
	outside.addPolygon( sides );
	return outside;
}

}
