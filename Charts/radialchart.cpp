#include "radialchart.h"
#include <QPainter>
#include <QDebug>

RadialChart::RadialChart( QWidget* parent ) : PointChart( parent ) {
  my_manualBounds = false;
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
  //paintAxis( &painter );
  paintChart( &painter );
}

void RadialChart::paintChart( QPainter* painter ) {
  painter->save();
  painter->setRenderHint( QPainter::Antialiasing );
  updateChart();

  qreal w = ( width() - 20 );
  qreal h = ( height() - 20 );

  w = qMin( w, h );
  QRect r( 10, 10, w , w );

  qreal angle = ( 360.0 / model()->rowCount() );
  qreal angleStep = ( 360.0 / model()->rowCount() );

  painter->setPen( Qt::red );
  painter->setBrush( Qt::red );


  /* Equation of coordinates transform */

  /* First step : real equation to transform values in coordinates */
  qreal m = r.width() /( my_max - my_min );
  qreal p = - m * my_min;

  /* Calculate an offset to avoid min = (0;0) */
  qreal offset = (r.width() * 0.5 - p)/m;
  qreal max = my_max + offset;

  /* Re-calculate equation with the offset */
  m = r.width() /( max - my_min );
  p = - m * my_min;

  qreal a = 0;

  QColor c = my_color;
  c.setAlpha( c.alpha() * 0.75 );
  painter->setBrush( c );
  painter->setPen( QPen( my_color, 2 ) );

  for ( int i = 0; i < model()->rowCount(); ++i ) {
    qreal v = model()->data( model()->index( i, 0 ) ).toReal();
    qreal w = m * (v + offset) + p;
    qreal h = m * (v + offset) + p;
    QPainterPath path;
    path.moveTo( r.center() );
    QRect r1( r.center().x() - w/2, r.center().y() - h/2, w, h );
    path.arcTo( r1, a + 1, angle - 2 );
    path.closeSubpath();
    painter->drawPath( path );
    a += angleStep;
  }

  int centerWidth = r.width()/2 * 0.1;
  painter->setPen( Qt::white );
  painter->setBrush( Qt::white );
  painter->drawEllipse( r.center(), centerWidth, centerWidth );

  a = 20;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    qreal v = model()->data( model()->index( i, 0 ) ).toReal();
    painter->drawText( QPointF(w + 20, a), QString::number( v ) );
    a+= 15;
  }

  painter->restore();
}
