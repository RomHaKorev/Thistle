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

#include "linearchart.h"
#include "../base/seriedelegates.h"
#include <QItemDelegate>
#include <QPainter>
#include <QPaintEvent>
#include <QItemDelegate>
#include <QDebug>
#include <QStyleOptionViewItem>

#include "../base/abstractcoordinatesystemview.h"
#include "../base/cartesiancoordinateplane.h"
namespace Thistle
{

LinearChart::LinearChart( QWidget* parent ) : SerieChart( new LinearChartPrivate( this ), parent )
{}

LinearChart::LinearChart( AbstractCoordinateSystemView* coordSysView, QWidget *parent )
	: SerieChart( new LinearChartPrivate( this, coordSysView ), parent )
{
	Q_D( LinearChart );
	this->setCoordSysView( coordSysView );
	d->pointDelegate->setParent( this );
	d->barDelegate->setParent( this );
}

LinearChart::LinearChart( LinearChartPrivate* d, QWidget* parent )
	: SerieChart( d, parent )
{
}

LinearChart::LinearChart( LinearChartPrivate* d, AbstractCoordinateSystemView* coordSysView, QWidget* parent )
	: SerieChart( new LinearChartPrivate( this, coordSysView ), parent )
{
	this->setCoordSysView( coordSysView );
	d->pointDelegate->setParent( this );
	d->barDelegate->setParent( this );
}


QPainterPath LinearChart::itemPath( const QModelIndex& index ) const
{
    const Q_D( LinearChart );

	QPainterPath path;

	CartesianCoordinatePlane* coordSys = dynamic_cast< CartesianCoordinatePlane* >( d->coordinateSystemView()->coordinateSystem() );

	if ( !coordSys )
		return path;

    QRectF r;
    Thistle::Types t = this->columnType( index.column() );
    bool ok = false;
    qreal value = index.data().toReal( &ok );
    
	if ( ok == false )
        return path;

	QPointF pos = coordSys->valueToPoint( index.row(), value );
    QList<int> orderedColumns = this->calculateColumnsOrder();
    if ( t == Thistle::Bar )
    {
        QList<int> bars = this->barStyleColumns();

		qreal stepSize = coordSys->axisLength( CartesianCoordinatePlane::X ) / this->model()->rowCount();

		qreal margin = stepSize * 0.1;
		qreal w = float( stepSize - margin ) / bars.count();
		pos += QPointF( margin / 2.0 + w * bars.indexOf( index.column() ) - stepSize / 2.0, 0 );

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
