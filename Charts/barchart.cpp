#include "barchart.h"

#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QDebug>

BarChart::BarChart( QWidget* parent ) : PointChart( parent ) {
  my_ratio = 1;
  my_part = 0;
}

BarChart::~BarChart() {

}
void BarChart::paintChart( QPainter *painter ) {
  qDebug() << Q_FUNC_INFO << my_part;
  updateChart();
  painter->save();
  painter->setRenderHint( QPainter::Antialiasing );

  painter->setBrush( my_color );
  painter->setPen( Qt::NoPen );
  //painter->setPen( QPen( QColor( 50, 50, 50 ), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
  /* Equation of coordinates transform */
  qreal m = -height() /( my_max - my_min );
  qreal p = height() - m * my_min;

  qreal stepW = ( width() / my_points.size() );
  qreal w = stepW / my_ratio - 5;

  foreach ( QPointF point, my_points ) {
    qreal h = p - point.y() + 1;
    QRect r( point.x() + w * my_part, point.y(), w - 1, h );
    painter->drawRect( r );
  }

  painter->setBrush( Qt::NoBrush );
  painter->setPen( QPen( QColor( my_color.red() + 50, my_color.green() + 50, my_color.blue() + 50 ), 4 ) );
  for ( int r = 0; r < model()->rowCount(); ++r ) {
    QModelIndex id = model()->index( r, my_col, rootIndex() );
    if ( selectionModel()->isSelected( id ) || currentIndex() == id ) {
      QPointF point = my_points.at( r );
      qreal h = p - point.y() + 1;
      QRect r( point.x() + w * my_part, point.y(), w - 1, h + 5 );
      QRegion reg = QRegion( 0, 0, width(), height() ).subtract( QRect( r.bottomLeft().x() - 3, r.bottomLeft().y() - 4, w + 6, 7 ) );
      painter->setClipRegion( reg );
      painter->drawRect( r );
    }
  }

  painter->restore();

}

void BarChart::setWidthRatio( int ratio, int part ) {
  if ( ratio > 0 ) {
    my_ratio = ratio;
  }
  my_part = part;
}
