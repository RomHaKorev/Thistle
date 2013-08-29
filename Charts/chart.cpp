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
  along with Marb.  If not, see <http://www.gnu.org/licenses/>.

 Marb  Copyright (C) 2013  Dimitry Ernot
*/

#include "chart.h"

#include <QFontMetrics>
#include <qmath.h>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>

Chart::Chart( QWidget* parent ) : MarbAbstractItemView( parent ) {
  myMin = 0;
  myMax = 0;
  myMinBound = 0;
  myMaxBound = 0;
  myOrder = 1;
  myNbTicks = 10;
  myTickSize = 0;
  myAlpha = 1;
  myBeta = 1;
  myChartRect = QRect();
  myValuesRect = QRect();
  myTitleRect = QRect();
  myTitle = "";
  myTitleRect = QRect();
  myOrigin = QPoint(0,0);
  myNbDigits = 1;
  myMarginX = 20;
  myMarginY = 20;
}


Chart::~Chart() {
}


void Chart::calculateBounds() {
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


void Chart::calculateLegendRect() {
    QFontMetrics metrics( font() );
    int h = metrics.height() + 5;
    int cols = this->model()->columnCount();
    int nbLines = 1;
    int w = 40;
    int maxWidth = myChartRect.width();
    for( int c = 0; c < cols; ++c ) {
      QString s( this->model()->headerData( c, Qt::Horizontal ).toString() );
      int sWidth = metrics.width( s ) + 40;
      if ( ( w + sWidth ) > maxWidth ) {
        ++nbLines;
        w = sWidth;
      } else {
        w += sWidth;
      }
    }
    myLegendRect = QRect( myMarginX, myMarginY, maxWidth, nbLines * h );
}


qreal Chart::calculateOrder( qreal value ) const {
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


ChartStyle Chart::columnStyle( int column ) const {
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

void Chart::defineRects() {
  myChartRect = QRect( QPoint( myMarginX, myMarginY ),
                       this->size() - QSize( myMarginX * 2, myMarginY * 2 ) );

  this->calculateLegendRect();
  myChartRect.setHeight( myChartRect.height() - myLegendRect.height() - 10 );
  myChartRect.translate( 0, myLegendRect.height() + 10 );

  if ( myTitle != "" ) {
    QFont font = this->font();
    font.setItalic( true );
    QFontMetrics metrics( font );
    QRect r( 0, 0, myChartRect.width() - 40, 0 );
    myTitleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );
    myChartRect.setHeight( myChartRect.height() - myTitleRect.height() - 20 );
  }
}


QModelIndex Chart::indexAt(const QPoint &point) const {
  if ( this->model() == 0 ) {
    return QModelIndex();
  }
  for( int row = 0; row < this->model()->rowCount(); ++row ) {
    for( int col = 0; col < this->model()->columnCount(); ++col ) {
      QModelIndex index = this->model()->index( row, col );
      QPainterPath r = this->itemPath( index );
      if ( r.contains( point ) ) {
        return index;
      }
    }
  }
  return QModelIndex();
}


QPainterPath Chart::itemPath(const QModelIndex &index) const {
  QPainterPath path;
  path.addRect( this->itemRect( index ) );
  return path;
}


/*Paint the legend for the given column. The kind of legend should be defined by each view (linear and radial represent legend differently).
The column legend is represented by a square colored with the pen and brush style and the column name.
*/
void Chart::paintColumnLegend( QPainter& painter, int column, QPoint pos, int metricsH ) {
  QRect r( pos.x() + 20, pos.y() - 10, 20, 20 );
  QPoint posText = pos + QPoint( 45, metricsH/2 );

  ChartStyle style = this->columnStyle( column );

  QString s = this->model()->headerData( column, Qt::Horizontal ).toString();
  painter.drawText( posText, s );
  painter.save();
  painter.setPen( style.pen() );
  painter.setBrush( style.brush() );
  painter.drawRect (r );
  painter.restore();
}


void Chart::paintEvent( QPaintEvent* event ) {
  if ( this->model() == 0 ) {
    return;
  }
    QPainter painter( viewport() );
    painter.setClipRect( event->rect() );
    paintChart( painter );
}


/*Paint the legend in the QRect self._legendRect
The legend corresponds to the text in the Horizontal QHeaderView and the style defined for each column.
*/
void Chart::paintLegend( QPainter& painter) {
    painter.save();
    QFontMetrics metrics( font() );
    int metricsH = metrics.height();
    int h = metricsH + 5;
    int cols = this->model()->columnCount();
    int w = 0;
    int maxWidth = myLegendRect.width();
    QPoint legendPos( myLegendRect.topLeft() );
    QPoint pos = legendPos + QPoint( 50, 0);
    for (int c = 0; c < cols; ++c ) {
      QString s( this->model()->headerData( c, Qt::Horizontal ).toString() );
      int sWidth = metrics.width( s ) + 50;
      QPoint p;
      if ( ( w + sWidth ) > maxWidth ) {
        int y = pos.y();
        p = QPoint( myLegendRect.x() + 10, y + h );
        pos = QPoint( myLegendRect.x() + 50, y + h );
        w = sWidth;
        pos += QPoint( sWidth, 0 );
      } else {
        p = pos + QPoint( -40,  0 );
        w += sWidth;
        pos += QPoint( sWidth, 0 );
      }
      this->paintColumnLegend(painter, c, p + QPoint(0, metricsH), metricsH);
    }
    painter.restore();
}


void Chart::process() {
  if ( this->model() == 0 ) {
    return;
  }
  myMin = 0;
  myMax = 0;
  myMinBottomMargin = this->scanValues() + 10.0;
  this->updateRects();
}


void Chart::resizeEvent(QResizeEvent * ev) {
  QAbstractItemView::resizeEvent( ev );
  this->updateValues();
}


bool Chart::save( QString filename ) {
  QPixmap pix( size() );
  pix.fill( Qt::white );
  QPainter painter( &pix );
  this->paintChart( painter );
  return pix.save( filename );
}


int Chart::scanValues() {
  int rows = this->model()->rowCount();
  int cols = this->model()->columnCount();
  QFontMetrics metrics( font() );
  int textWidth = 0;
  for (int r = 0; r < rows; ++r ) {
    QString s = this->model()->headerData( r, Qt::Vertical ).toString();
    textWidth = qMax( textWidth, metrics.width( s ) + 5 );
    for (int c = 0; c < cols; ++c ) {
      qreal value = this->model()->index( r, c ).data().toReal();
      myMin = qMin( myMin, value );
      myMax = qMax( myMax, value );
    }
  }
  return textWidth;
}


void Chart::setAlphaBeta() {
  myAlpha = -qreal(myValuesRect.height()) / ( myMaxBound - myMinBound );
  myBeta = (myMaxBound * myValuesRect.height() ) / ( myMaxBound - myMinBound ) + myValuesRect.y();
  myOrigin.setY( myBeta );
}


void Chart::setColumnStyle( int column, ChartStyle style ) {
    myStyle[ column ] = style;
}


void Chart::setModel( QAbstractItemModel* model ) {
  QAbstractItemView::setModel( model );
  this->process();
}


void Chart::setScrollBarValues() {
}


void Chart::setTitle( QString title ) {
  myTitle = title;
  this->updateRects();
}


void Chart::updateValues() {
  this->process();
}


qreal Chart::valueToPx( qreal value) const {
  return value * myAlpha + myBeta;
}
