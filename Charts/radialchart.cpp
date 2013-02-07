#include "radialchart.h"
#include <QPainter>
#include <QDebug>

RadialChart::RadialChart( QWidget* parent ) : PointChart( parent ) {
  my_manualBounds = false;
}

void RadialChart::setModel( QAbstractItemModel* model ) {
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
    if ( !myHiddenColumns.contains( i ) ) {
      l << i;
    }
  }
  mySpec.calculate( l );
  updateMinMax();

  /* Equation of coordinates transform */
  myLine.setP1( QPointF( mySpec.min, 5.0/100.0 * myRect.width()) );
  myLine.setP2( QPointF( mySpec.max, myRect.width()) );

  mySpec.m = (0.9) / (mySpec.max - mySpec.min );
  mySpec.p = -mySpec.m * mySpec.min + 0.1;
}

void RadialChart::resizeEvent( QResizeEvent* ev ) {

  QAbstractItemView::resizeEvent( ev );

  mySpec.chartSize = QSize( width() - 40 - mySpec.yLabelsLength, height() - 40 );
  updateChart();
}

void RadialChart::paintEvent( QPaintEvent* ) {
  qreal w = ( width() - 20 );
  qreal h = ( height() - 20 );

  if ( w < h ) {
    myRect = QRect( 10, 10 + (h-w)/2, w , w );
  } else {
    myRect = QRect( 10 + (w-h)/2, 10, h , h );
  }

  QPainter painter(viewport());
  painter.setRenderHint( QPainter::Antialiasing );

  paintChart( painter );

  painter.setPen( QPen( my_color, 2 ) );
  int centerWidth = myRect.width()/2 * 0.1;
  painter.setBrush( Qt::white );
  painter.drawEllipse( myRect.center(), centerWidth, centerWidth );
}

void RadialChart::paintChart( QPainter& painter ) {

  painter.save();
  painter.setRenderHint( QPainter::Antialiasing );
  updateChart();

  paintGrid( painter );

  qreal delta = ( 360.0 / model()->rowCount() );
  delta /= ( model()->columnCount() - myHiddenColumns.count() );

  qreal angle = 0;

  for ( int i = 0; i < model()->rowCount(); ++i ) {
    for ( int j = 0; j < model()->columnCount(); ++j ) {
      if ( !myHiddenColumns.contains( j ) ) {
        QModelIndex index = model()->index( i, j );
        paintValue( painter, index, delta, angle );
        angle += delta;
      }
    }
  }

  paintAxis( painter );

  painter.restore();
}

void RadialChart::paintValue( QPainter& painter, QModelIndex& index, qreal delta, qreal angle ) {
  painter.save();
  QColor c = myColors.value( index.column(), Clint::predefinedColor( index.column() ) );
  painter.setPen( QPen( c, 2 ) );
  c.setAlpha( c.alpha() * 0.75 );
  painter.setBrush( c );

  qreal v = model()->data( index ).toReal();
  qreal w = mySpec.valueToPixel( v );
  w = myLine.pointAt( w ).y();
  QPainterPath path;
  path.moveTo( myRect.center() );
  QRect r1( myRect.center().x() - w/2, myRect.center().y() - w/2, w, w );
  path.arcTo( r1, angle + 1, delta - 2 );
  path.closeSubpath();
  painter.drawPath( path );
  painter.restore();
}

void RadialChart::paintGrid( QPainter& painter ) {
  painter.save();
  painter.setPen( QPen( Qt::lightGray, 2 ) );

  QPoint start( myRect.center() );
  QPoint end( myRect.center().x() + myRect.width()/2, myRect.center().y() );
  for ( qreal i = mySpec.gridStartValue() + mySpec.yStep; i < mySpec.max;
        i +=  mySpec.yStep ) {
    qreal radius = mySpec.valueToPixel( i );
    radius = myLine.pointAt( radius ).y();

    painter.drawEllipse( myRect.center(), radius/2.0, radius/2.0 );
  }
  painter.restore();
}

void RadialChart::paintAxis( QPainter& painter ) {
  painter.save();

  painter.setPen( QColor( 0x303030 ) );

  QPoint start( myRect.center() );
  QPoint end( myRect.center().x() + myRect.width()/2, myRect.center().y() );
  painter.drawLine( start, end );
  for ( qreal i = mySpec.gridStartValue() + mySpec.yStep; i < mySpec.max;
        i +=  mySpec.yStep ) {
    qreal radius = mySpec.valueToPixel( i );
    radius = myLine.pointAt( radius ).y();
    painter.drawText( start + QPoint( radius/2, 0 ), QString::number(i) );
  }
  painter.restore();
}

void RadialChart::setColumnVisible( int column, bool visible ) {
  myHiddenColumns.removeAll( column );
  if ( visible == false ) {
    myHiddenColumns << column;
  }
  update();
}
