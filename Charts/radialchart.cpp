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
  paintChart( &painter );
}

void RadialChart::paintChart(QPainter *painter) {
  painter->save();
  painter->setRenderHint( QPainter::Antialiasing );
  updateChart();

  painter->setPen( QPen( Qt::black, 2 ) );

  qreal w = ( width() - 20 );
  qreal h = ( height() - 20 );

  QRect r( 10, 10, w , h );
  //painter->drawRect( r );

  qreal angle = 0.8 * ( 360.0 / model()->rowCount() );
  qreal angleStep = ( 360.0 / model()->rowCount() );

  painter->setPen( Qt::NoPen );
  painter->setBrush( Qt::red );

  /* Equation of coordinates transform */
  qreal mW = -r.width() /( my_max - my_min );
  qreal pW = r.width() - mW * my_min;

  qreal mH = -r.height() /( my_max - my_min );
  qreal pH = r.height() - mH * my_min;

  qreal a = 0;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    qreal v = model()->data( model()->index( i, 0 ) ).toReal();
    qreal w = mW * v + pW;
    qreal h = mH * v + pH;
    QPainterPath path;
    path.moveTo( r.center() );
    QRect r1( r.center().x() - w/2, r.center().y() - h/2, w, h );
    path.arcTo( r1, a, angle );
    path.closeSubpath();
    painter->drawPath( path );
    //painter->drawRect( r1 );

    a += angleStep;


  }
  painter->restore();
}
