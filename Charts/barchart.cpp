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

  QColor c = my_color;
  c.setAlpha( c.alpha() * 0.75 );


  //painter->setPen( QPen( QColor( 50, 50, 50 ), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
  /* Equation of coordinates transform */
  qreal m = -height() /( my_max - my_min );
  qreal p = height() - m * my_min;

  qreal stepW = ( width() / my_points.size() );
  qreal w = stepW / my_ratio - 5;

  painter->setBrush( c );
  painter->setPen( QPen( my_color, 2 ) );
  for ( int r = 0; r < model()->rowCount(); ++r ) {
    QModelIndex id = model()->index( r, my_col, rootIndex() );
    if ( selectionModel()->isSelected( id ) || currentIndex() == id ) {
      QPointF point = my_points.at( r );
      qreal h = p - point.y() + 1;
      QRect r;

      QRegion reg;
      if ( p - point.y() >= 0 ) {
        r = QRect( point.x() + w * my_part, point.y(), w - 1, h + 5 );
        reg = QRegion( 0, 0, width(), height() ).subtract( QRect( r.bottomLeft().x() - 3, r.bottomLeft().y() - 4, w + 6, 7 ) );
      } else {
        r = QRect( point.x() + w * my_part, point.y(), w, h - 5 );

        //reg = QRegion( 0, 0, width(), height() ).subtract( QRect( r.topLeft(), QSize( r.width(), 5 ) ) );
        painter->save();
        painter->setBrush( Qt::red );
        painter->setPen( Qt::NoPen );
        painter->drawRects( reg.rects() );
        painter->restore();
        painter->drawRect( r );
        return;
      }
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
