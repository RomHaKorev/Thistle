#include "linechart.h"

#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QDebug>

LineChart::LineChart( QWidget* parent ) : PointChart( parent ) {
}

LineChart::~LineChart() {

}
void LineChart::paintChart( QPainter *painter ) {

  PointChart::paintChart( painter );

  painter->save();
  painter->setRenderHint( QPainter::Antialiasing );

  /*if ( my_pointShape != Chart::None ) {
    QRegion region( QRect( QPoint(0, 0), size() ) );

    foreach( QPointF p, my_points ) {
      QRegion r( QRect( p.toPoint(), QSize( 12, 12) ).translated( -6, -6 ), QRegion::Ellipse );
      region = region.subtract( r );
    }

    painter->setClipRegion( region );
  }*/

  painter->setPen( QPen( my_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
  painter->drawPolyline( my_points );
  painter->restore();

}
