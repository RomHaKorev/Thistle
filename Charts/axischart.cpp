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

#include "Axis/axis.h"

#include "../kernel/abstractitemview_p.h"
#include "axischart_p.h"

namespace Thistle {

/*! Constructs an abstract Axis chart with the given \a parent.
*/
AxisChart::AxisChart( QWidget* parent ) : AbstractChart( new AxisChartPrivate( this ), parent ) {
}

AxisChart::AxisChart( AxisChartPrivate* d, QWidget* parent ) : AbstractChart( d, parent ) {
}

AxisChart::~AxisChart() {
}

/*!
Returns the current \a Axis instance used by this view.
*/
Axis* AxisChart::axis() const {
    const Q_D( AxisChart );
    return d->axis;
}

/*!
Calculates the order in which the series (columns) should be drawn.

For example, in \a LinearChart, the columns with the \s type \a Thistle::Bar will be drawn at first to avoid to mask the others.
*/
QList<int> AxisChart::calculateColumnsOrder() const {
    QList<int> l;
    for ( int i = 0; i < this->model()->columnCount(); ++i ) {
        l << i;
    }
    return ( l );
}

/*!
Return the \a SerieFormat used for the \c column.
*/
SerieFormat AxisChart::serieFormat( int column ) const {
    const Q_D( AxisChart );
    if ( d->style.contains( column ) ) {
            return d->style[ column ];
    }
    SerieFormat style;
    QColor c1 =  Global::predefinedLightColor( column );
    QColor c2 = Global::predefinedDarkColor( column );
    style.setPen( QPen( QColor(c2), 2 ) );
    style.setBrush( QBrush(c1) );
    return style;
}


void AxisChart::defineRects() {
    Q_D( AxisChart );
    d->axis->setChartRect( this->contentsRect() );
    this->calculateLegendRect( d->axis->chartRect() );
    d->axis->chartRect().setHeight( d->axis->chartRect().height() - d->legend->area.height() - 10 );
    d->axis->chartRect().translate( 0, d->legend->area.height() + 10 );

    if ( d->title != "" ) {
        QFont font = this->font();
        font.setItalic( true );
        QFontMetrics metrics( font );
        QRect r( 0, 0, d->axis->chartRect().width() - 40, 0 );
        d->titleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, d->title );
        d->axis->chartRect().setHeight( d->axis->chartRect().height() - d->titleRect.height() - 20 );
    }
}


/*Paint the legend for the given column. The kind of legend should be defined by each view (linear and radial represent legend differently).
The column legend is represented by a square colored with the pen and brush style and the column name.
*/
void AxisChart::paintSerieLegend( QPainter& painter, int serie, QPoint pos, int metricsH ) const {
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


/*void AxisChart::paintEvent( QPaintEvent* event ) {
    if ( this->model() == 0 ) {
        return;
    }
        QPainter painter( viewport() );
        painter.setClipRect( event->rect() );
        paintChart( painter );
}*/

/*!
Scans values in the model to find the minimum and the maximum. Returns the width needed to display the Y scale.
If the values are greater than zero, the minimum is equal to 0. If the values are less than 0, the maximum is equal to 0.
If a value is not a number (undefined, a string, etc.), she's considered as equal to 0.
*/
void AxisChart::scan() {
    Q_D( AxisChart );
    int rows = this->model()->rowCount();
    int cols = this->model()->columnCount();
    //QFontMetrics metrics( d->axis->font() );
    QFontMetrics metrics( this->font() );
    int textWidth = 0;
    int valueWidth = 0;
    qreal value = this->model()->index( 0, 0 ).data().toDouble();
    qreal min = 0;
    qreal max = 0;
    for ( int r = 0; r < rows; ++r ) {
        QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
        textWidth = qMax( textWidth, metrics.width( s ) + 5 );
        for ( int c = 0; c < cols; ++c ) {
            value = this->model()->index( r, c ).data().toDouble();
            min = float( qMin( min, value ));
            max = float( qMax( max, value ));
            QString s = QString::number( value, 'f', d->axis->nbDigits() );
            valueWidth = qMax( valueWidth, metrics.width( s ) );
        }
    }
    d->axis->setMin( min );
    d->axis->setMax( max );
    d->axis->setTickSize( qMax( d->axis->calculateOrder( min ), d->axis->calculateOrder( max) ) );
    d->axis->setYLabelsLength( valueWidth + 5 );
    d->axis->setXLabelsLength( textWidth );
}

/*!
Sets the \a Axis use by the view to determine the item's positions.
*/
void AxisChart::setAxis( Axis* axis ) {
    Q_D( AxisChart );
    d->axis = axis;
}


/*!
Sets the given \c style to the given \c column.
The \c style defines the look and feel of a serie.
*/
void AxisChart::setSerieFormat( int column, SerieFormat style ) {
    Q_D( AxisChart );
    d->style[ column ] = style;
}


void AxisChart::setModel( QAbstractItemModel* model ) {
    Q_D( AxisChart );
    QAbstractItemView::setModel( model );
    d->axis->setModel( model );
    this->process();
}


void AxisChart::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command ) {
    QRect contentsRect = rect.translated(
                    this->horizontalScrollBar()->value(),
                    this->verticalScrollBar()->value()).normalized();
    int rows = this->model()->rowCount( this->rootIndex() );
    QList<int> columns = this->calculateColumnsOrder();
    int count = 0;
    QPainterPath contentsPath;
    contentsPath.addRect( contentsRect );
    for ( int row = 0; row < rows; ++row ) {
        Q_FOREACH( int col, columns ) {
            QModelIndex index = this->model()->index( row, col, this->rootIndex() );
            QPainterPath path = this->itemPath( index );
            if ( !path.intersected(contentsPath).isEmpty() ) {
                count += 1;
                this->selectionModel()->select( index, command );
            }
        }
    }
    if ( count == 0 ) {
        this->selectionModel()->clear();
    }
    this->viewport()->update();
}

}