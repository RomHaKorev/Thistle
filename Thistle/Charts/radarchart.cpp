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

#include "radarchart.h"
#include "Axis/radaraxis.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

namespace Thistle
{

RadarChart::RadarChart( QWidget* parent ) : LinearChart( new RadarChartPrivate( this ), parent )
{
    Q_D( RadarChart );
    d->radarAxis = new RadarAxis();

    this->setAxis( d->radarAxis );
}

RadarChart::RadarChart( RadarChartPrivate* d, QWidget* parent ) : LinearChart( d, parent )
{
    d->radarAxis = new RadarAxis();

    this->setAxis( d->radarAxis );
}


QRectF RadarChart::itemRect(const QModelIndex &index) const
{
    Q_UNUSED( index )
    return QRect();
}


QPainterPath RadarChart::itemPath( const QModelIndex& index ) const
{
    /*Returns the bounding path for the item pointed by the [index].
    *index: QModelIndex*/
    const Q_D( RadarChart );
    QPainterPath path;
    float angle = float(d->radarAxis->stepSize() / this->model()->columnCount() );
    float delta = 0.1 * angle;
    float startAngle = angle * index.column();
    startAngle += index.row() * d->radarAxis->stepSize() + d->radarAxis->startAngle();
    QPainterPath pathCenter;
    QRect rectangle( -d->radarAxis->centerHoleDiam()/2, -d->radarAxis->centerHoleDiam()/2, d->radarAxis->centerHoleDiam(), d->radarAxis->centerHoleDiam() );
    rectangle.translate( d->radarAxis->valuesRect().center() );
    pathCenter.addEllipse( rectangle );
    qreal value = index.data().toReal();

    rectangle = d->radarAxis->valueToRect( value );

    path.moveTo( d->radarAxis->valuesRect().center() );
    path.arcTo( rectangle, (startAngle + delta), (angle - delta * 2) );
    path.closeSubpath();
    path = path.subtracted( pathCenter );
    return path;
}


void RadarChart::updateRects()
{
    Q_D( RadarChart );
    if ( this->model() == 0 )
    {
        return;
    }
    d->radarAxis->setModel( this->model() );
    this->scan();
    this->defineRects();
    qreal w = qMin( d->radarAxis->chartRect().width(), d->radarAxis->chartRect().height() );
    d->radarAxis->setValuesRect( QRect( -w/2, -w/2, w, w ) );
    d->radarAxis->valuesRect().translate( d->radarAxis->chartRect().center().x(), d->radarAxis->chartRect().center().y() );
    d->titleRect.moveTo( d->radarAxis->chartRect().bottomLeft() );
    d->titleRect.translate( (d->radarAxis->chartRect().width() - d->titleRect.width())/2, 10 );

    d->radarAxis->update();
}

}