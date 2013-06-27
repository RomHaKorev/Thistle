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

#include "radialchart.h"

#include <QPainter>
#include <QPaintEvent>

RadialChart::RadialChart( QWidget* parent ) : Chart( parent ) {
  myOrigin = QPointF(20, 20);
  myCenterHoleDiam = 0;
  myNbTicks = 5;
}

QRectF RadialChart::itemRect(const QModelIndex &index) const {
  Q_UNUSED( index )
  return QRect();
}

void RadialChart::process() {
  if ( model() == 0 ) {
    return;
  }
  myMin = 0;
  myMax = 0;
  processSpec();
}

void RadialChart::setAlphaBeta() {
  qreal w = myValuesRect.width();
  myCenterHoleDiam =  w * 0.2;
  qreal m = w * 0.3;
  myAlpha = float( w - m )  / float( myMaxBound - myMinBound );
  myBeta = w - myAlpha * myMaxBound;
}

void RadialChart::processSpec() {
  if ( model() == 0 ) {
    return;
  }

  myChartRect = QRect( QPoint( 20, 20 ), size() - QSize( 40, 40 ) );

  QFontMetrics metrics( font() );

  if ( myTitle != "" ) {
    QRect r( 0, 0, myCenterHoleDiam, 0 );
    myTitleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );
    myChartRect.setHeight( myChartRect.height() - myTitleRect.height() );
  }

  int w = qMin( myChartRect.width(), myChartRect.height() );

  myValuesRect = QRect( -w/2, -w/2, w, w );
  myValuesRect.translate( myChartRect.center().x(), myChartRect.center().y() );

  scanValues();
  calculateBounds();

  myX = 360.0 / qreal( model()->rowCount() );

  setAlphaBeta();

  myTitleRect.translate( myValuesRect.center().x(), myChartRect.height() + 30 );
}

void RadialChart::paintTextAxis( QPainter& painter) const {
  QFontMetrics metrics( font() );
  int h = metrics.height();

  qreal x = myMinBound;
  while ( x <= myMaxBound ) {
    qreal v = valueToPx( x )/2;
    int w = metrics.width( QString::number( x ) );
    painter.save();
    painter.setPen( QPen( QColor( Marb::DarkGray ), 1.5 ) );
    painter.rotate( -90 );
    painter.translate( -myValuesRect.center().y(), myValuesRect.center().x() + v );
    painter.drawText( -w - 5, h / 2, QString::number(x) );
    x += myTickSize;
    painter.restore();
  }
}

void RadialChart::paintValues( QPainter& painter, int column ) const {
  int rows = model()->rowCount();
  painter.save();

  qreal angle = ( myX / model()->columnCount() );

  qreal startAngle = angle * column + 10;

  QPainterPath pathCenter;
  QRect rectangle( -myCenterHoleDiam/2, -myCenterHoleDiam/2, myCenterHoleDiam, myCenterHoleDiam );
  rectangle.translate( myValuesRect.center() );

  pathCenter.addEllipse( rectangle );


  for (int r = 0; r < rows; ++r ) {
    QModelIndex index = model()->index( r, column );
    qreal y = valueToPx( index.data( Qt::DisplayRole ).toReal() );
    QRect rectangle( -y/2, -y/2, y, y );
    rectangle.translate( myValuesRect.center() );
    QPainterPath path;
    path.moveTo( myValuesRect.center() );
    path.arcTo( rectangle, startAngle + 1, angle - 2 );
    path.closeSubpath();
    path = path.subtracted( pathCenter );

    painter.drawPath( path );

    startAngle += myX;
  }

  painter.restore();
}

void RadialChart::paintAxis( QPainter& painter ) const {
  painter.save();
  paintTicks( painter );
  painter.setRenderHint( QPainter::Antialiasing, false );
  painter.setPen( QPen( Qt::darkGray, 1.5 ) );
  QPoint p1 = myValuesRect.center();
  QPoint p2 = p1 + QPoint( myValuesRect.width()/2, 0 );
  painter.drawLine( p1, p2 );
  painter.restore();

}

void RadialChart::paintTicks( QPainter& painter ) const {
  qreal y = myMinBound;
  painter.save();
  QColor c = QColor( Qt::lightGray );
  c.setAlpha( 100 );
  painter.setPen( QPen( c , 1.5) );

  while ( y <= myMaxBound ) {
    qreal v = valueToPx( y );
    QRect rectangle( -v/2, -v/2, v, v );
    rectangle.translate( myValuesRect.center() );
    QPainterPath path;
    path.addEllipse( rectangle );
    painter.drawPath( path );
    y += myTickSize;
  }

  painter.restore();
}


void RadialChart::paintChart(QPainter& painter) {
  painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );

  int cols = model()->columnCount();
  painter.save();
  paintAxis( painter );
  for ( int c = 0; c < cols; ++c ) {
    ChartStyle style = columnStyle( c );
    painter.setPen( style.pen() );
    painter.setBrush( style.brush() );
    paintValues( painter, c );
  }

  painter.restore();
  paintTextAxis( painter );

  painter.drawText( myTitleRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );

}
