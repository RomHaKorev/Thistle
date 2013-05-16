#include "piechart3d.h"

#include "Marb.h"
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

PieChart3D::PieChart3D( QWidget* parent ) :
  PieChart( parent ) {
  //mySplitted = true;
  myRender = Translucent;
  //myRender = Plain;
}

void PieChart3D::updateChart() {
  PieChart::updateChart();
  if ( mySplitted == true ) {
    myRect.translate( 10, 10 );
    myRect.setWidth( myRect.width() - 20 );
    myRect.setHeight( myRect.height() - 20 );
  }

  myHeight = myRect.height() * 0.20;
  myRect.setHeight( myRect.height() * 0.75 );

  myAngles.clear();
  qreal angle = 0;
  for ( int i = 0; i < model()->rowCount(); ++i ) {
    myAngles << angle;
    QModelIndex index = model()->index( i, 0 );
    qreal v = qAbs( model()->data( index ).toReal() );
    qreal delta = 360.0 * v/myTotal;
    myAngles << delta;
    angle += delta;
  }
  myAngles << 360.0;
}

void PieChart3D::paintEvent(QPaintEvent *event) {
  QPainter painter( viewport() );
  painter.save();
  painter.setRenderHint( QPainter::Antialiasing );

  updateChart();

  if ( myRender != Plain ) {
    paintExternal( painter, true );
    paintSides( painter );
    paintExternal( painter, false );
  } else {
    paintExternal( painter, false );
  }

  for ( int i = 0; i < myAngles.count() - 2; i+=2 ) {
    QModelIndex index = model()->index( i/2, 0 );
    QColor color( model()->data( index, Qt::DecorationRole ).toString() );
    if ( !color.isValid() ) {
      color = Marb::predefinedColor( i/2 );
    }

    bool isSelected = selectionModel()->selectedIndexes().contains( index )
                      || currentIndex() == index;

    if ( mySplitted == false ) {
      paintPart( painter, myAngles[i], myAngles[i + 1], color, isSelected );
    } else {
      paintPartSplitted( painter, myAngles[i], myAngles[i + 1], color, isSelected );
    }
  }
  painter.restore();
}

void PieChart3D::paintExternal( QPainter& painter, bool top ) {
  for ( int i = 0; i < myAngles.count() - 2; i+=2 ) {
    QModelIndex index = model()->index( i/2, 0 );
    QColor color( model()->data( index, Qt::DecorationRole ).toString() );
    if ( !color.isValid() ) {
      color = Marb::predefinedColor( i/2 );
    }
    painter.save();
    qreal a1 = myAngles[i];
    qreal delta = myAngles[i + 1];
    qreal a2 = a1 + delta;

    if ( a1 < 180 && a2 > 180 ) {
      paintLeft( painter, color );
    }

    if ( top == false ) {
      if ( a1 <= 180 && a2 > 180 ) {
        if ( myRender == Plain ) {
          configureColor( painter, color, 1 );
          qreal delta = 180.0 - a1;
          QPointF offset = splittedOffset( myAngles[i], delta );
          QPainterPath path = itemExternalPart( myAngles[i], delta, mySplitted );
          painter.drawPath( path );
          painter.setPen( Qt::NoPen );
          path = side( 180, offset );
          painter.drawPath( path );
          painter.restore();
        }
        continue;
      }
      if ( a1 > 180 || a2 > 180 ) {
        painter.restore();
        continue;
      }
    } else {
      if ( a1 < 180 && a2 < 180 ) {
        painter.restore();
        continue;
      }
    }

    QPainterPath path = itemExternalPart( a1, delta, mySplitted );

    configureColor( painter, color, 3 );
    painter.drawPath( path );
    painter.restore();
  }
}


void PieChart3D::paintSides( QPainter& painter ) {
  QList<QPair<QPainterPath, QColor> > rightBottom;
  QList<QPair<QPainterPath, QColor> > rightTop;
  QList<QPair<QPainterPath, QColor> > leftBottom;
  QList<QPair<QPainterPath, QColor> > leftTop;

  for ( int i = 0; i < myAngles.count() - 2; i+=2 ) {
    QModelIndex index = model()->index( i/2, 0 );
    QColor color( model()->data( index, Qt::DecorationRole ).toString() );
    if ( !color.isValid() ) {
      color = Marb::predefinedColor( i/2 );
    }

    QPointF offset = splittedOffset( myAngles[i], myAngles[i + 1] );

    QPainterPath path = side( myAngles[i], offset );

    if ( myAngles[i] <= 90 ) {
      rightBottom << QPair<QPainterPath, QColor>(path, color);
    } else if ( myAngles[i] <= 180 ) {
      leftBottom.prepend( QPair<QPainterPath, QColor>(path, color) );
    } else if ( myAngles[i] <= 270 ) {
      leftTop.prepend( QPair<QPainterPath, QColor>(path, color) );
    } else {
      rightTop << QPair<QPainterPath, QColor>(path, color);
    }

    path = side( myAngles[i] + myAngles[i+1], offset );

    if ( myAngles[i] <= 90 ) {
      rightBottom << QPair<QPainterPath, QColor>(path, color);
    } else if ( myAngles[i] <= 180 ) {
      leftBottom.prepend( QPair<QPainterPath, QColor>(path, color) );
    } else if ( myAngles[i] <= 270 ) {
      leftTop.prepend( QPair<QPainterPath, QColor>(path, color) );
    } else {
      rightTop << QPair<QPainterPath, QColor>(path, color);
    }
  }

  QPair<QPainterPath, QColor> pair;
  leftTop << rightTop << leftBottom << rightBottom;
  qDebug() << leftTop.size();
  foreach( pair, leftTop ) {
    configureColor( painter, pair.second, 2 );
    painter.drawPath( pair.first );
  }
}

void PieChart3D::configureColor(QPainter &painter, QColor base, int flag) const {
  painter.setPen( QPen( base, 2 ) );

  switch( flag ) {
    case 2:
      base = base.darker( 110 );
    break;
    case 3:
      base = base.darker( 120 );
    default:
    break;
  }

  switch( myRender ) {
    case PieChart3D::Plain:
      painter.setPen( QPen( base.darker( 110 ), 2 ) );
    break;
    case PieChart3D::Translucent:
      base.setAlpha( 125 );
    break;
    case PieChart3D::WireFrame:
      base.setAlpha( 10 );
    break;
  }
  painter.setBrush( base );
}

QPainterPath PieChart3D::side( qreal angle, QPointF centerOffset ) const {
  QPainterPath ell;

  QRectF r = myRect;
  if (mySplitted == true ) {
    r.translate( centerOffset.x(), centerOffset.y() );
  }
  ell.addEllipse( r );
  QPointF p1 = ell.pointAtPercent( angle / 360.0 );
  QPointF center = r.center();
  QPointF offset = QPointF( 0, myHeight );

  QPainterPath outside;
  outside.setFillRule( Qt::WindingFill );


  QPolygonF sides;
  sides << center << p1 << p1 + offset << center + offset << center;

  outside.moveTo( center );
  outside.addPolygon( sides );

  return outside;
}

QPainterPath PieChart3D::itemSidesPath( const QModelIndex& index ) const {
  QPainterPath outside;

  qreal angle = myStartAngle;
  for ( int r = 0; r < index.row(); ++r ) {
    QModelIndex id = model()->index( r, 0 );
    qreal v = qAbs( model()->data( id ).toReal() );
    qreal delta = 360.0 * v/myTotal;
    angle += delta;
  }

  qreal v = qAbs( model()->data( index ).toReal() );
  qreal delta = 360.0 * v/myTotal;

  if ( selectionModel()->selectedIndexes().contains( index )) {
    outside = itemExternalPart( angle, delta, true );
  } else {
    outside = itemExternalPart( angle, delta );
  }
  return outside;
}

QPainterPath PieChart3D::itemExternalPart( qreal angle, qreal delta, bool splitted ) const {
  QPainterPath ell;
  ell.addEllipse( myRect );
  QPointF p1 = ell.pointAtPercent( angle / 360.0 );
  QPointF offset = QPointF( 0, myHeight );

  QPainterPath outside;
  outside.setFillRule( Qt::WindingFill );

  outside.moveTo( p1 );
  outside.arcTo( myRect, -angle, -delta );
  outside.lineTo( outside.currentPosition() + offset );
  outside.arcTo( myRect.translated( 0, myHeight ), -angle - delta, delta );
  outside.lineTo( p1 );

  if ( splitted == true ) {
    QPointF p = splittedOffset( angle, delta );
    outside.translate( p.x(), p.y() );
  }

  return outside;
}

void PieChart3D::setRender( PieChart3D::Render r ) {
  myRender = r;
}


void PieChart3D::paintLeft( QPainter& painter, QColor color ) {
  if ( myRender == WireFrame ) {
    return;
  }
  painter.save();
  color.setAlpha( 125 );
  configureColor( painter, color, 0 );
  int width = painter.pen().width()/2;
  painter.setPen( Qt::NoPen );
  QPainterPath path;
  QPainterPath ellipse1, ellipse2;
  ellipse1.addEllipse( myRect );
  ellipse2.addEllipse( myRect.translated( 0, myHeight ) );
  path.moveTo( ellipse1.pointAtPercent( 0.5 ) + QPointF( -width, 0 ) );
  path.lineTo( ellipse2.pointAtPercent( 0.5 ) + QPointF( -width, 0 ) );
  path.arcTo( myRect.translated( 0, myHeight ), 180, -90 );
  path.moveTo( ellipse1.pointAtPercent( 0.5 ) );
  path.arcTo( myRect, 180, 90 );
  path = path.subtracted( ellipse1 );

  painter.fillPath( path, color );
  painter.restore();
}
