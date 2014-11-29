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

#include "axischart.h"

#include <QFontMetrics>
#include <qmath.h>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QMargins>

#include <QDebug>

#include "Axis/axisview.h"

#include "../kernel/abstractitemview_p.h"
#include "axischart_p.h"
#include "../kernel/global.h"

namespace Thistle
{

AxisChart::AxisChart( QWidget* parent ) : AbstractChart( new AxisChartPrivate( this ), parent )
{}

AxisChart::AxisChart( AxisChartPrivate* d, QWidget* parent ) : AbstractChart( d, parent )
{}

AxisChart::~AxisChart()
{}

AxisView* AxisChart::axisView() const
{
    const Q_D( AxisChart );
    return d->axisView;
}


QList<int> AxisChart::calculateColumnsOrder() const
{
    QList<int> l;
    for ( int i = 0; i < this->model()->columnCount(); ++i )
    {
        l << i;
    }
    return ( l );
}


void AxisChart::defineRects()
{
    Q_D( AxisChart );

    QRect rect( this->contentsRect() );
    this->calculateLegendRect( rect );
    rect.setHeight( rect.height() - d->legend->area.height() - 10 );
    rect.translate( 0, d->legend->area.height() + 10 );
    d->axisView->setRect( rect );
    /*d->axisView->setChartRect( this->contentsRect() );
    this->calculateLegendRect( d->axisView->chartRect() );
    d->axisView->chartRect().setHeight( d->axisView->chartRect().height() - d->legend->area.height() - 10 );
    d->axisView->chartRect().translate( 0, d->legend->area.height() + 10 );*/

    if ( d->title != "" )
    {
        QFont font = this->font();
        font.setItalic( true );
        QFontMetrics metrics( font );
        QRect r( 0, 0, rect.width() - 40, 0 );
        d->titleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, d->title );
        rect.setHeight( rect.height() - d->titleRect.height() - 20 );
        d->axisView->setRect( rect );
    }
}


void AxisChart::paintSerieLegend( QPainter& painter, int serie, QPoint pos, int metricsH ) const
{
    QRect r( pos.x() + 20, pos.y() - 10, 20, 20 );
    QPoint posText = pos + QPoint( 45, metricsH/2 );

    SerieFormat style = this->serieFormat( serie );

    QString s = this->model()->headerData( serie, Qt::Horizontal ).toString();
    painter.drawText( posText, s );
    painter.save();
    painter.setPen( style.pen() );
    painter.setBrush( style.brush() );
    painter.drawRect (r );
    painter.restore();
}


void AxisChart::scan()
{
    Q_D( AxisChart );
    d->readModel();
}


void AxisChart::setAxisView( AxisView* axis )
{
    Q_D( AxisChart );
    d->axisView = axis;
}


void AxisChart::setModel( QAbstractItemModel* model )
{
    Q_D( AxisChart );
    QAbstractItemView::setModel( model );
    d->axisView->setModel( model );
    this->process();
}


void AxisChart::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command )
{
    QRect contentsRect = rect.translated(
                             this->horizontalScrollBar()->value(),
                             this->verticalScrollBar()->value()).normalized();
    int rows = this->model()->rowCount( this->rootIndex() );
    QList<int> columns = this->calculateColumnsOrder();
    int count = 0;
    QPainterPath contentsPath;
    contentsPath.addRect( contentsRect );
    for ( int row = 0; row < rows; ++row )
    {
        Q_FOREACH( int col, columns )
        {
            QModelIndex index = this->model()->index( row, col, this->rootIndex() );
            QPainterPath path = this->itemPath( index );
            if ( !path.intersected(contentsPath).isEmpty() )
            {
                count += 1;
                this->selectionModel()->select( index, command );
            }
        }
    }
    if ( count == 0 )
    {
        this->selectionModel()->clear();
    }
    this->viewport()->update();
}

}