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

AxisChart::AxisChart( QWidget* parent ) : AbstractChart( parent ) {
    myMin = 0;
    myMax = 0;
    myMinBound = 0;
    myMaxBound = 0;
    myOrder = 1;
    myNbTicks = 10;
    myTickSize = 0;
    myTitleRect = QRect();
    myTitle = "";
    myTitleRect = QRect();
    myOrigin = QPoint(0,0);
    myNbDigits = 1;
}


AxisChart::~AxisChart() {
}


Axis* AxisChart::axis() const {
    return myAxis;
}

void AxisChart::calculateBounds() {
    myMinBound = myMin;
    myMaxBound = myMax;
    if ( myMaxBound == myMinBound ) {
        ++myMaxBound;
        --myMinBound;
    }
    myOrder = calculateOrder( myMax - myMin );
    myTickSize = (myMax - myMin ) / (myNbTicks - 1);
    if ( myOrder >= 10 ) {
        myNbDigits = 0;
    } else if ( myOrder == 1 ) {
        myNbDigits = 2;
    } else {
        int nbZero = QString::number(myOrder).count( "0" );
        myNbDigits = nbZero + 2;
    }
}


qreal AxisChart::calculateOrder( qreal value ) const {
    qreal order = 1.0;
    qreal v = qAbs( value );
    if ( v >= 1 ) {
        while ( v > 1 ) {
            order *= 10.0;
            v /= 10.0;
        }
        order /= 10.0;
    } else if ( v != 0 ) {
        while ( v < 1 ) {
            order /= 10.0;
            v *= 10.0;
        }
        order *= 10.0;
    }
    return order;
}


ChartStyle AxisChart::columnStyle( int column ) const {
    if ( myStyle.contains( column ) ) {
            return myStyle[ column ];
    }
    ChartStyle style;
    QColor c1 =Marb::predefinedLightColor( column );
    QColor c2 = Marb::predefinedDarkColor( column );
    style.setPen( QPen( QColor(c2), 2 ) );
    style.setBrush( QBrush(c1) );
    return style;
}

void AxisChart::defineRects() {
    myAxis->chartRect = this->contentsRect();
    this->calculateLegendRect( myAxis->chartRect );
    myAxis->chartRect.setHeight( myAxis->chartRect.height() - myLegendRect.height() - 10 );
    myAxis->chartRect.translate( 0, myLegendRect.height() + 10 );

    if ( myTitle != "" ) {
        QFont font = this->font();
        font.setItalic( true );
        QFontMetrics metrics( font );
        QRect r( 0, 0, myAxis->chartRect.width() - 40, 0 );
        myTitleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );
        //myAxis->chartRect.setHeight( myAxis->chartRect.height() - myTitleRect.height() - 20 );
        myAxis->chartRect.setHeight( myAxis->chartRect.height() - myTitleRect.height() - 20 );
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
    int rows = this->model()->rowCount();
    int cols = this->model()->columnCount();
    //QFontMetrics metrics( myAxis->font() );
    QFontMetrics metrics( this->font() );
    int textWidth = 0;
    int valueWidth = 0;
    qreal value = this->model()->index( 0, 0 ).data().toDouble();
    qreal _min = 0;
    qreal _max = 0;
    for ( int r = 0; r < rows; ++r ) {
        QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
        textWidth = qMax( textWidth, metrics.width( s ) + 5 );
        for ( int c = 0; c < cols; ++c ) {
            value = this->model()->index( r, c ).data().toDouble();
            _min = float( qMin( _min, value ));
            _max = float( qMax( _max, value ));
            valueWidth = qMax( valueWidth, metrics.width( QString::number( value ) ) );
        }
    }
    myAxis->min = _min;
    myAxis->max = _max;
    myAxis->tickSize = qMax( myAxis->calculateOrder( _min ), myAxis->calculateOrder( _max) );
    myAxis->yLabelsLength = valueWidth;
    myAxis->xLabelsLength = textWidth;
    return 0;
}


void AxisChart::setAxis( Axis* axis ) {
    myAxis = axis;
}


void AxisChart::setColumnStyle( int column, ChartStyle style ) {
        myStyle[ column ] = style;
}


void AxisChart::setModel( QAbstractItemModel* model ) {
    QAbstractItemView::setModel( model );
    myAxis->setModel( model );
    this->process();
}