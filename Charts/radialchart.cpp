#include "radialchart.h"
#include <QPainter>
#include <QDebug>

RadialChart::RadialChart( QWidget* parent ) : PointChart( parent ) {
  my_manualBounds = false;
  qreal w = ( width() - 20 );
  qreal h = ( height() - 20 );

  w = qMin( w, h );
  myRect = QRect( 10, 10, w , w );
}

void RadialChart::updateChart() {
  my_points.clear();
  if ( model() == 0 || model()->rowCount() == 0 ) {
    return;
  }

  updateMinMax();
}

void RadialChart::paintEvent(QPaintEvent *) {
  QPainter painter(viewport());
  painter.setRenderHint( QPainter::Antialiasing );
  paintChart( &painter );

  painter.setPen( QPen( my_color, 2 ) );
  int centerWidth = myRect.width()/2 * 0.1;
  painter.setBrush( Qt::white );
  painter.drawEllipse( myRect.center(), centerWidth, centerWidth );

  int a = 20;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    qreal v = model()->data( model()->index( i, 0 ) ).toReal();
    painter.drawText( QPointF(myRect.width() + 20, a), QString::number( v ) );
    a+= 15;
  }
}

void RadialChart::paintChart( QPainter* painter ) {
  painter->save();
  painter->setRenderHint( QPainter::Antialiasing );
  updateChart();

  qreal angle = ( 360.0 / model()->rowCount() );
  qreal angleStep = ( 360.0 / model()->rowCount() );

  /* Equation of coordinates transform */

  /* First step : real equation to transform values in coordinates */
  qreal m = myRect.width() /( my_max - my_min );
  qreal p = - m * my_min;

  /* Calculate an offset to avoid min = (0;0) */
  qreal offset = (myRect.width() * 0.5 - p)/m;
  qreal max = my_max + offset;

  /* Re-calculate equation with the offset */
  myM = myRect.width() /( max - my_min );
  myP = - m * my_min;

  qreal a = 0;

  QColor c = my_color;
  c.setAlpha( c.alpha() * 0.75 );
  painter->setBrush( c );
  painter->setPen( QPen( my_color, 2 ) );

  for ( int i = 0; i < model()->rowCount(); ++i ) {
    qreal v = model()->data( model()->index( i, 0 ) ).toReal();
    qreal w = myM * (v + offset) + myP;
    qreal h = myM * (v + offset) + myP;
    QPainterPath path;
    path.moveTo( myRect.center() );
    QRect r1( myRect.center().x() - w/2, myRect.center().y() - h/2, w, h );
    path.arcTo( r1, a + 1, angle - 2 );
    path.closeSubpath();
    painter->drawPath( path );
    a += angleStep;
  }
  painter->restore();
}
