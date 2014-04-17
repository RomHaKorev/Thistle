  /*
 This file is part of Marb.

    Marb is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    Marb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with Marb.    If not, see <http://www.gnu.org/licenses/>.

 Marb    Copyright (C) 2013    Dimitry Ernot & Romha Korev
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

namespace Marb {

AxisChart::AxisChart( QWidget* parent ) : AbstractChart( new AxisChartPrivate(), parent ) {
}

AxisChart::AxisChart( AxisChartPrivate* d, QWidget* parent ) : AbstractChart( d, parent ) {
}

AxisChart::~AxisChart() {
}


Axis* AxisChart::axis() const {
    const Q_D( AxisChart );
    return d->axis;
}


ChartStyle AxisChart::columnStyle( int column ) const {
    const Q_D( AxisChart );
    if ( d->style.contains( column ) ) {
            return d->style[ column ];
    }
    ChartStyle style;
    QColor c1 =Color::predefinedLightColor( column );
    QColor c2 = Color::predefinedDarkColor( column );
    style.setPen( QPen( QColor(c2), 2 ) );
    style.setBrush( QBrush(c1) );
    return style;
}

void AxisChart::defineRects() {
    Q_D( AxisChart );
    d->axis->setChartRect( this->contentsRect() );
    this->calculateLegendRect( d->axis->chartRect() );
    d->axis->chartRect().setHeight( d->axis->chartRect().height() - d->legendRect.height() - 10 );
    d->axis->chartRect().translate( 0, d->legendRect.height() + 10 );

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

    ChartStyle style = this->columnStyle( serie );

    QString s = this->model()->headerData( serie, Qt::Horizontal ).toString();
    painter.drawText( posText, s );
    painter.save();
    painter.setPen( style.pen() );
    painter.setBrush( style.brush() );
    painter.drawRect (r );
    painter.restore();
}


void AxisChart::paintEvent( QPaintEvent* event ) {
    if ( this->model() == 0 ) {
        return;
    }
        QPainter painter( viewport() );
        painter.setClipRect( event->rect() );
        paintChart( painter );
}


int AxisChart::scan() {
    /*Scans values in the model to find the minimum and the maximum. Returns the width needed to display the Y scale.
    If the values are greater than zero, the minimum is equal to 0. If the values are less than 0, the maximum is equal to 0.
    If a value is not a number (undefined, a string, etc.), she's considered as equal to 0. */

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
    return 0;
}


void AxisChart::setAxis( Axis* axis ) {
    Q_D( AxisChart );
    d->axis = axis;
}


void AxisChart::setColumnStyle( int column, ChartStyle style ) {
    Q_D( AxisChart );
    d->style[ column ] = style;
}


void AxisChart::setModel( QAbstractItemModel* model ) {
    Q_D( AxisChart );
    QAbstractItemView::setModel( model );
    d->axis->setModel( model );
    this->process();
}

}