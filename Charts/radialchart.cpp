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
  paintAxis( &painter );
  paintChart( &painter );

}

void RadialChart::paintAxis( QPainter* painter ) {
  painter->save();
  painter->setPen( QPen( Qt::lightGray, 1.5) );

  qreal w = ( width() - 20 );
  qreal h = ( height() - 20 );

  w = qMin( w, h );
  QRect r( 10, 10, w , w );

  qreal angle = ( 360.0 / model()->rowCount() );
  qreal angleStep = ( 360.0 / model()->rowCount() );

  QPainterPath path;
  path.addEllipse( r.center(), r.width() / 2, r.width() / 2 );
  painter->drawPath( path );

  qreal a = 0;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    QPainterPath path;
    path.moveTo( r.center() );
    QRect r1( r.center().x() - r.width()/2, r.center().y() - r.height()/2, r.width(), r.height() );
    path.arcTo( r1, a, angle );
    //path.closeSubpath();
    painter->drawPath( path );
    a += angleStep;
  }
  painter->restore();
}

void RadialChart::paintChart( QPainter* painter ) {
  painter->save();
  painter->setRenderHint( QPainter::Antialiasing );
  updateChart();

  painter->setPen( Qt::lightGray );

  qreal w = ( width() - 20 );
  qreal h = ( height() - 20 );

  w = qMin( w, h );
  QRect r( 10, 10, w , w );

  qreal angle = ( 360.0 / model()->rowCount() );
  qreal angleStep = ( 360.0 / model()->rowCount() );

  painter->setPen( Qt::red );
  painter->setBrush( Qt::red );

  /* Equation of coordinates transform */
  qreal mW = -r.width() /( my_max - my_min );
  qreal pW = r.width() - mW * my_min;

  qreal mH = -r.width() /( my_max - my_min );
  qreal pH = r.width() - mH * my_min;

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
    a += angleStep;
  }

  a = 20;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    qreal v = model()->data( model()->index( i, 0 ) ).toReal();
    painter->drawText( QPointF(500, a), QString::number( v ) );
    a+= 15;
  }

  painter->restore();
}
