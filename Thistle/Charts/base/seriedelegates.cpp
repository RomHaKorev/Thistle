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

#include "seriedelegates.h"
#include "../base/seriechart.h"
#include "../../kernel/global.h"
#include "../base/serieformat.h"
#include <QPainter>
#include <QDebug>

namespace Thistle
{

DotDelegate::DotDelegate( SerieChart* parent ) : AbstractChartDelegate( parent )
{}

QPolygon DotDelegate::createDiamond( const QRect& rect ) const
{
    QPolygon poly;
    poly.append( rect.topLeft() + QPoint(rect.width()/2, 0) );
    poly.append( rect.topRight() + QPoint(0, rect.height()/2) );
    poly.append(rect.bottomLeft() + QPoint( rect.width()/2, 0 ));
    poly.append(rect.topLeft() + QPoint( 0, rect.height()/2 ));
    return poly;
}


void DotDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( option.state == QStyle::State_Off )
    {
        this->paintDisabled(painter, option, index);
    }
    else
    {
        this->paintEnabled(painter, option, index);
    }
}


void DotDelegate::paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    SerieChart* p = qobject_cast<SerieChart*>( this->parent() );
    QRect r = option.rect;
    QRect r2 = QRect( r.topLeft() - QPoint( 5, 5 ), r.bottomRight() + QPoint( 5, 5 ) );
    QColor c;
    if ( p != 0 )
    {
        SerieFormat style = p->serieFormat( index.column() );
        c = style.brush().color();
        c.setAlpha( 125 );
    }
    else
    {
        QColor c( painter->pen().color() );
        c.setAlpha( 75 );
    }

    painter->setPen( Qt::NoPen );
    QRadialGradient gradient( r2.center(), r2.width() / 2.0, r2.center() );
    gradient.setColorAt( 0, c );
    gradient.setColorAt( 1, Qt::transparent );
    gradient.setSpread( QRadialGradient::ReflectSpread );
    painter->setBrush( QBrush( gradient ) );
    painter->drawEllipse( r2 );
    painter->restore();
}


void DotDelegate::paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    SerieChart* p = qobject_cast<SerieChart*>( this->parent() );
    SerieFormat style;
    if ( p != 0 ) style = p->serieFormat( index.column() );
    painter->save();
    painter->setBrush( style.brush() );
    painter->setPen( style.pen() );
    QRect r = option.rect;

    if ( style.shape() == Thistle::Ellipse )
    {
        painter->drawEllipse( r );
    }
    else if ( style.shape() == Thistle::RoundedRect )
    {
        painter->drawRoundedRect( r, 5, 5 );
    }
    else if ( style.shape() == Thistle::Diamond )
    {
        painter->drawPolygon( createDiamond( r ) );
    }
    else if ( style.shape() == Thistle::Triangle )
    {
        QPolygon poly;
        poly.append( r.topLeft() + QPoint( r.width()/2, 0 ) );
        poly.append( r.bottomLeft() );
        poly.append( r.bottomRight() );
        painter->drawPolygon( poly );
    }
    else if ( style.shape() == Thistle::ReversedTriangle )
    {
        QPolygon poly;
        poly.append( r.bottomLeft() + QPoint( r.width()/2, 0 ) );
        poly.append( r.topLeft() );
        poly.append( r.topRight() );
        painter->drawPolygon( poly );
    }
    else
    {
        painter->drawRect( r );
    }
    painter->restore();
}




BarDelegate::BarDelegate(SerieChart* parent) : AbstractChartDelegate( parent )
{
}


void BarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( option.state == QStyle::State_Off )
    {
        this->paintDisabled(painter, option, index);
    }
    else
    {
        this->paintEnabled(painter, option, index);
    }
}


void BarDelegate::paintDisabled(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED( index )
    QRect r = option.rect.normalized();

    painter->save();

    painter->setPen( Qt::NoPen );
    QLinearGradient gradient( r.topLeft() + QPoint( r.width()/2, 0 ), r.topLeft()    );
    gradient.setSpread( QLinearGradient::ReflectSpread );
    QColor c( Qt::gray );
    c.setAlpha( 100 );
    gradient.setColorAt( 0, c );
    gradient.setColorAt( 0.4, c );
    gradient.setColorAt( 1, Qt::transparent );
    painter->setBrush( QBrush( gradient ) );
    painter->setClipRect( r );
    painter->drawRect( r );

    painter->restore();
}


void BarDelegate::paintEnabled(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    SerieChart* p = qobject_cast<SerieChart*>( this->parent() );
    SerieFormat style;
    if ( p != 0 ) style = p->serieFormat( index.column() );
    QRect r = option.rect;

    painter->save();

    painter->setBrush( style.brush() );
    painter->setPen( Qt::NoPen );
    painter->drawRect( r );

    QPolygon polygon;
    if ( option.decorationPosition == QStyleOptionViewItem::Top )   // Negative value
    {
        polygon.append( r.bottomLeft() );
        polygon.append( r.topLeft() );
        polygon.append( r.topRight() );
        polygon.append( r.bottomRight() );
    }
    else
    {
        polygon.append( r.topLeft() );
        polygon.append( r.bottomLeft() );
        polygon.append( r.bottomRight() );
        polygon.append( r.topRight() );
    }

    painter->setBrush( Qt::NoBrush );
    QPen pen( style.pen() );
    pen.setCapStyle( Qt::FlatCap );
    painter->setPen( pen );

    painter->drawPolyline( polygon );

    painter->restore();
}

}
