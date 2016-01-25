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
along with Thistle.
If not, see <http://www.gnu.org/licenses/>.
Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#include "linechart.h"
#include "../base/seriedelegates.h"
#include "../base/abstractcoordinatesystemview.h"
#include "../base/cartesiancoordinateplane.h"
#include "../base/linearaxisdelegate.h"

#include <QItemDelegate>
#include <QPainter>
#include <QPaintEvent>
#include <QItemDelegate>
#include <QStyleOptionViewItem>


namespace Thistle
{

LineChart::LineChart( QWidget* parent ) : SerieChart( new LineChartPrivate( this ), parent )
{
	QSharedPointer<Thistle::LinearAxisDelegate> delegate( new Thistle::LinearAxisDelegate( this->coordinateSystemView() ) );
	LinearAxisDelegate::TickStyles styles = LinearAxisDelegate::Long;
	styles|= LinearAxisDelegate::Label;
	delegate->setTickStyles( styles );
	this->coordinateSystemView()->setDelegateForAxis( 0, delegate );
}

LineChart::LineChart( AbstractCoordinateSystemView* coordSysView, QWidget *parent )
    : SerieChart( new LineChartPrivate( this, coordSysView ), parent )
{
    Q_D( LineChart );
	this->setCoordSysView( coordSysView );
	d->pointDelegate->setParent( this );
	d->barDelegate->setParent( this );
	QSharedPointer<Thistle::LinearAxisDelegate> delegate( new Thistle::LinearAxisDelegate( this->coordinateSystemView() ) );
	LinearAxisDelegate::TickStyles styles = LinearAxisDelegate::Long;
	styles|= LinearAxisDelegate::Label;
	delegate->setTickStyles( styles );
	this->coordinateSystemView()->setDelegateForAxis( 0, delegate );
}

LineChart::LineChart( LineChartPrivate* d, QWidget* parent )
	: SerieChart( d, parent )
{
	QSharedPointer<Thistle::LinearAxisDelegate> delegate( new Thistle::LinearAxisDelegate( this->coordinateSystemView() ) );
	LinearAxisDelegate::TickStyles styles = LinearAxisDelegate::Long;
	styles|= LinearAxisDelegate::Label;
	delegate->setTickStyles( styles );
	this->coordinateSystemView()->setDelegateForAxis( 0, delegate );
}

LineChart::LineChart( LineChartPrivate* d, AbstractCoordinateSystemView* coordSysView, QWidget* parent )
    : SerieChart( new LineChartPrivate( this, coordSysView ), parent )
{
	this->setCoordSysView( coordSysView );
	d->pointDelegate->setParent( this );
	d->barDelegate->setParent( this );
	QSharedPointer<Thistle::LinearAxisDelegate> delegate( new Thistle::LinearAxisDelegate( this->coordinateSystemView() ) );
	LinearAxisDelegate::TickStyles styles = LinearAxisDelegate::Long;
	styles|= LinearAxisDelegate::Label;
	delegate->setTickStyles( styles );
	this->coordinateSystemView()->setDelegateForAxis( 0, delegate );
}


QPainterPath LineChart::itemPath( const QModelIndex& index ) const
{
    const Q_D( LineChart );
	QPainterPath path;
	CartesianCoordinatePlane* coordSys = dynamic_cast< CartesianCoordinatePlane* >( d->coordinateSystemView()->coordinateSystem() );

	if ( !coordSys )
		return path;

	QRectF r;
	Thistle::Types t = this->serieFormat( index.column() ).type();
	bool ok = false;
	qreal value = index.data().toReal( &ok );

	if ( ok == false )
		return path;

	QPointF pos = coordSys->valueToPoint( index.row(), value );
	if ( t == Thistle::Bar )
	{
		QList<int> bars = this->barStyleColumns();
		qreal lgt = coordSys->axisLength( CartesianCoordinatePlane::X );
		qreal stepSize = lgt / this->model()->rowCount();
		qreal margin = (stepSize * 0.1) / bars.count();
		stepSize -= margin * 2;
		qreal w = float( stepSize - margin ) / bars.count();
		/*if ( w > 40 )
			w = 40;*/
		pos += QPointF( (margin / 2.0 + w) * bars.indexOf( index.column() ) - stepSize / 2.0, 0 );

		QPointF br( pos.x() + w, d->coordinateSystemView()->origin().y() );
		r = QRectF( pos, br );

		if ( value < 0 )
			r.translate( 0, 1 );
		else
			r.translate( 0, -1 );
	}
	else
		r = QRectF( -7, -7, 14 ,14 ).translated( pos.x(), pos.y() );

	path.addRect( r.normalized() );
	return path;
}

}
