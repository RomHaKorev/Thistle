#include "radialchart.h"
#include <QPainter>
#include <QDebug>

RadialChart::RadialChart( QWidget* parent ) : PointChart( parent ) {
  my_manualBounds = false;
}

void RadialChart::setModel(QAbstractItemModel *model) {
  PointChart::setModel( model );
  mySpec.model = model;
}

void RadialChart::updateChart() {
  my_points.clear();
  if ( model() == 0 || model()->rowCount() == 0 ) {
    return;
  }
  QList<int> l;
  for( int i = 0; i < model()->columnCount(); ++i ) {
    l << i;
  }
  mySpec.calculate( l );
  updateMinMax();

  /* Equation of coordinates transform */
  /* First step : real equation to transform values in coordinates */
  qreal m = myRect.width() /( my_max - my_min );
  qreal p = - m * my_min;

  /* Calculate an offset to avoid min = (0;0) */
  myOffset = (myRect.width() * 0.2 - p)/m;
  qreal max = my_max + myOffset;

  /* Re-calculate equation with the offset */
  mySpec.m = myRect.width() /( max - my_min );
  mySpec.p = - mySpec.m * my_min;

  mySpec.m = (myRect.width() - myRect.width()*0.2) / ( my_max - my_min );
  mySpec.p = -mySpec.m * my_min;
  qDebug() << my_min << mySpec.p;
  myLine.setP1( QPointF(my_min, 0.2 * myRect.width()) );
  myLine.setP2( QPointF(my_max, myRect.width()) );
}

void RadialChart::resizeEvent(QResizeEvent * ev) {

  QAbstractItemView::resizeEvent( ev );

  mySpec.chartSize = QSize( width() - 40 - mySpec.yLabelsLength, height() - 40 );
  updateChart();
}

void RadialChart::paintEvent(QPaintEvent *) {
  qDebug() << Q_FUNC_INFO;
  qreal w = ( width() - 20 );
  qreal h = ( height() - 20 );

  w = qMin( w, h );
  myRect = QRect( 10, 10, w , w );

  QPainter painter(viewport());
  painter.setRenderHint( QPainter::Antialiasing );
  paintChart( painter );
  paintAxis( painter );

  painter.drawRect( myRect );

  /*painter.setPen( QPen( my_color, 2 ) );
  int centerWidth = myRect.width()/2 * 0.1;
  painter.setBrush( Qt::white );
  painter.drawEllipse( myRect.center(), centerWidth, centerWidth );

  int a = 20;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    qreal v = model()->data( model()->index( i, 0 ) ).toReal();
    painter.drawText( QPointF(myRect.width() + 20, a), QString::number( v ) );
    a+= 15;
  }*/
}

void RadialChart::paintChart( QPainter& painter ) {
  qDebug() << Q_FUNC_INFO;
  qreal w = ( width() - 20 );
  qreal h = ( height() - 20 );

  w = qMin( w, h );
  myRect = QRect( 10, 10, w , w );

  painter.save();
  painter.setRenderHint( QPainter::Antialiasing );
  updateChart();

  qreal angle = ( 360.0 / model()->rowCount() );
  qreal angleStep = ( 360.0 / model()->rowCount() );

  qreal a = 0;

  QColor c = my_color;
  c.setAlpha( c.alpha() * 0.75 );
  painter.setBrush( c );
  painter.setPen( QPen( my_color, 2 ) );

  for ( int i = 0; i < model()->rowCount(); ++i ) {
    qreal v = model()->data( model()->index( i, 0 ) ).toReal();
    qreal w = mySpec.valueToPixel(v + myOffset);
    QPainterPath path;
    path.moveTo( myRect.center() );
    QRect r1( myRect.center().x() - w/2, myRect.center().y() - w/2, w, w );
    path.arcTo( r1, a + 1, angle - 2 );
    path.closeSubpath();
    painter.drawPath( path );
    a += angleStep;
    painter.drawText( path.pointAtPercent( 0.5 ), QString::number( v ) );
  }

  painter.setPen( QPen( my_color, 2 ) );
  int centerWidth = myRect.width()/2 * 0.1;
  painter.setBrush( Qt::white );
  painter.drawEllipse( myRect.center(), centerWidth, centerWidth );

  a = 20;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    qreal v = model()->data( model()->index( i, 0 ) ).toReal();
    painter.drawText( QPointF(myRect.width() + 20, a), QString::number( v ) );
    a+= 15;
  }
  painter.restore();
}

void RadialChart::paintAxis( QPainter& painter ) {
  qDebug() << Q_FUNC_INFO << mySpec.gridStartValue() + mySpec.yStep << mySpec.max << mySpec.yStep;
  painter.save();

  QPoint start( myRect.center() );
  QPoint end( myRect.center().x() + myRect.width()/2, myRect.center().y() );
  painter.drawLine( start, end );

  for ( qreal i = mySpec.gridStartValue() + mySpec.yStep; i < mySpec.max;
        i +=  mySpec.yStep ) {
    qreal radius = myLine.pointAt( i/my_max ).y();
    //= mySpec.valueToPixel( i + myOffset );
    painter.drawEllipse( myRect.center(), radius/2, radius/2 );
    qDebug() << "radius" << i << radius;
    painter.drawText( start + QPoint( radius/2, 0 ), QString::number(i) );
    /*start.setY( mySpec.valueToPixel( i ) );
    end.setY( mySpec.valueToPixel( i ) );
    painter.setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
    painter.drawLine( start, end );
    painter.setPen(QPen(Qt::gray, 0, Qt::SolidLine));
    QPointF p = start - QPoint( mySpec.yLabelsLength + 3, 0 );
    painter.drawText( p, QString::number( i ) );*/
    //mySpec.valueToPixel()
  }
  painter.restore();
}
