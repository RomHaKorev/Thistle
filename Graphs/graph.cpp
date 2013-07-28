#include "graph.h"

#include <QScrollBar>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>

#include "../marbitemdelegate.h"

static const qreal Pi = 3.14159265358979323846264338327950288419717;


Graph::Graph(QWidget *parent) : MarbAbstractItemView(parent), myTimer(this) {
  //setFixedSize( 500, 300 );
  myTimer.setInterval(1);
  connect( &myTimer, SIGNAL(timeout()), this, SLOT(processTimer()) );
  MarbItemDelegate* delegate = new MarbItemDelegate( this );
  MarbStyle style = delegate->itemStyle();
  style.setShape( Marb::Ellipse );
  delegate->setItemStyle( style );
  setItemDelegate( delegate );
  myMovableItem = true;
  myElasticItem = true;
  myWeight = 9.81;
}


QRectF Graph::itemRect(const QModelIndex &index) const {
  QPointF pos = myItemPos.value( index ).pos();
  return QRect(-20,-20,40,40).translated( pos.x(), pos.y() );
}


void Graph::addEdge( QModelIndex idx1, QModelIndex idx2, Edge::Type type ) {
  if ( ( idx1 == idx2 ) || !idx1.isValid() || !idx2.isValid() ) {
    return;
  }
  myEdges << Edge( idx1, idx2, type );
  updateValues();
}

void Graph::addEdge( int row1, int col1, int row2, int col2, Edge::Type type ) {
  if ( model() == 0 ) {
    return;
  }
  addEdge( model()->index( row1, col1 ), model()->index( row2, col2 ), type );
}

void Graph::setScrollBarValues() {

}


QModelIndex Graph::indexAt(const QPoint &point) const {
  QPoint p = point - QPoint( horizontalScrollBar()->value(), verticalScrollBar()->value() );
  foreach( QModelIndex id, myItemPos.keys() ) {
    QRectF r = itemRect( id );
    if ( r.contains( p ) ) {
      return id;
    }
  }
  return QModelIndex();
}


void Graph::updateValues() {
  if ( this->model() == 0 ) {
    return;
  }

  myItemPos.clear();
  int rows = this->model()->rowCount();
  int nbRows = qFloor( qSqrt( rows ) );

  int x = 10;
  int y = 10;
  int i = 0;
  for( int r = 0; r < rows; ++ r ) {
    QModelIndex idx = this->model()->index( r, 0 );
    if ( idx.isValid() && idx.data().isValid() ) {
      Node n;
      n.setPos( QPointF( x, y ) );
      //n.setPos(10 + qrand() % 200, 10 + qrand() % 200);
      myItemPos.insert( idx, n );
      x += 100;
      ++i;
      if ( i > nbRows ) {
        x = 10;
        y += 100;
        i = 0;
      }
    }
  }

  foreach ( Edge e, myEdges ) {
    myItemPos[ e.leftIndex ].addEdge( &(myItemPos[ e.rightIndex ]) );
    myItemPos[ e.rightIndex ].addEdge( &(myItemPos[ e.leftIndex ]) );
  }

  myTimer.start();
  oldLength = 0;
}


void Graph::paintEvent( QPaintEvent* /*event*/ ) {
  QPainter painter(viewport());
  painter.setRenderHint( QPainter::Antialiasing );
  paintEdges( painter );
  paintItems( painter );
}


void Graph::paintEdges( QPainter& painter, QPointF offset ) {
  painter.save();
  painter.setPen( QPen( QColor( Marb::Gray ), 3 ) );
  painter.setBrush( QColor( Marb::Gray ) );
  foreach( Edge edge, myEdges ) {
    paintEdge( painter, edge.leftIndex,  edge.rightIndex, edge.type );
  }
  painter.restore();
}


void Graph::paintEdge( QPainter& painter, QModelIndex idx1, QModelIndex idx2, Edge::Type type ) {
  QRectF r1 = itemRect( idx1 );
  QRectF r2 = itemRect( idx2 );
  QPointF p1 = r1.center();
  QPointF p2 = r2.center();
  QLineF line( p1, p2  );

  if ( type == Edge::NoArrow ) {
    painter.drawLine( line );
    return;
  }

  int i = 0;
  QList<QPointF> l;
  QPointF p;
  l << r1.topLeft() << r1.topRight() << r1.bottomRight() << r1.bottomLeft() << r1.topLeft();
  while ( i < ( l.size() - 1 ) ) {
    if ( line.intersect( QLineF( l[i], l[i + 1] ), &p ) == QLineF::BoundedIntersection ) {
      p1 = p;
      break;
    }
    ++i;
  }
  i = 0;
  l.clear();
  l << r2.topLeft() << r2.topRight() << r2.bottomRight() << r2.bottomLeft() << r2.topLeft();
  while ( i < ( l.size() - 1 ) ) {
    if ( line.intersect( QLineF( l[i], l[i + 1] ), &p ) == QLineF::BoundedIntersection ) {
      p2 = p;
      break;
    }
    ++i;
  }
  line = QLineF( p1, p2  );
  if ( type == Edge::Bilateral ) {

    painter.drawLine( line.pointAt( 0.1 ), line.pointAt( 0.9 ) );
    paintArrow( painter, QLineF( p1, p2 ) );
    paintArrow( painter, QLineF( p2, p1 ) );
  } else {
    painter.drawLine( line.p1(), line.pointAt( 0.9 ) );
    paintArrow( painter, QLineF( p1, p2 ) );
  }
}


void Graph::paintArrow( QPainter& painter, QLineF line ) {
  QPen originalPen = painter.pen();
  QPen pen = painter.pen();
  pen.setWidth( 1 );
  painter.setPen( pen );
  QPointF p1 = line.p2();
  QLineF l( p1, line.pointAt( (line.length() - 15)/line.length() ) );
  l.setAngle( l.angle() + 30 );
  QPointF p2 = l.p2();
  l = QLineF( p1, line.pointAt( (line.length() - 15)/line.length() ) );
  l.setAngle( l.angle() - 30 );
  QPointF p3 = l.p2();
  painter.drawPolygon( QPolygonF() << p1 << p2 << p3 );
  painter.setPen( originalPen );
}


void Graph::paintItems( QPainter& painter, QPointF offset ) {
  foreach( QModelIndex idx, myItemPos.keys()  ) {
    QStyleOptionViewItem option;
    option.rect = itemRect( idx ).translated( offset.x(), offset.y() ).toRect();
    itemDelegate()->paint( &painter, option, idx );
  }
}


void Graph::calculateForces( QModelIndex& idx ) {
  Node* node = &( myItemPos[ idx ] );

  /* Calculate forces pushing the items */
  QPointF velocity( 0, 0 );
  foreach ( Node n, myItemPos.values() ) {
    QPointF vec( node->pos() - n.pos() );
    qreal dx = vec.x();
    qreal dy = vec.y();
      double l = 2.0 * (dx * dx + dy * dy);
      if (l > 0) {
          velocity += QPointF( (dx * 500.0) / l, (dy * 500.0) / l );
      }
  }

  /* Calculate forces pulling items relative to their weight */
  qreal weight = ( node->edges().size() + 1 ) * myWeight;
  foreach (Node* n, node->edges()) {
      QPointF vec( node->pos() - n->pos() );
      velocity -= QPointF( vec.x() / weight, vec.y() / weight );
  }

  /* Do not apply velocity if it's too low */
  if (qAbs(velocity.x()) < 0.1 && qAbs(velocity.y()) < 0.1) {
    velocity = QPointF( 0, 0 );
  }

  if ( idx != myMovedItem ) {
    node->setPos( node->pos() + velocity );
  } else {
  }
}


void Graph::processTimer() {
  if ( myItemPos.isEmpty() ) {
    myMovedItem = QModelIndex();
    myTimer.stop();
    return;
  }

  foreach( QModelIndex idx, myItemPos.keys() ) {
    calculateForces( idx );
    calculateForces( idx );
  }

  QPolygonF poly;
  foreach ( Node n, myItemPos.values() ) {
    poly << n.pos();
  }
  QPointF offset = QPointF( 50, 50 ) - poly.boundingRect().topLeft();
  foreach ( QModelIndex idx, myItemPos.keys() ) {
    myItemPos[ idx ].setPos( myItemPos[ idx ].pos() + offset );
  }

  myRealSize = poly.boundingRect().size() + 2 * ( itemRect( model()->index( 0, 0 ) ).size() );

  update();
  viewport()->update();
  qreal length = 0;

  QList<QPointF> pts;
  foreach( Node n, myItemPos.values() ) {
    pts << n.pos();
  }

  QPointF p1 = pts[0];
  for ( int i = 1; i < pts.size(); ++i ) {
    length += QLineF( p1, pts[i] ).length();
    p1 = pts[i];
  }
  if ( qAbs(length - oldLength ) > 0.0001 ) {
    oldLength = length;
  } else {
    save( "graph2.png" );
    myTimer.stop();
  }
}


bool Graph::save( QString filename ) {
  QSizeF s = myRealSize + QSize( 20, 20 );
  QPixmap pix( s.toSize() );
  pix.fill( Qt::transparent );
  QPainter painter( &pix );
  painter.setRenderHint( QPainter::Antialiasing );
  paintEdges( painter );
  paintItems(painter );
  painter.end();
  return pix.save( filename );
}


void Graph::mousePressEvent( QMouseEvent* event ) {
  if ( myMovableItem == true ) {
    QModelIndex idx = indexAt( event->pos() );
    if ( idx.isValid() ) {
      myMovedItem = idx;
    }
  }
  MarbAbstractItemView::mousePressEvent( event );
}


void Graph::mouseReleaseEvent( QMouseEvent* event ) {
  MarbAbstractItemView::mouseReleaseEvent( event );
  if ( myMovableItem == true ) {
    if ( myElasticItem == true ) {
      myTimer.start();
    } else {
      myMovedItem = QModelIndex();
    }
  }
}


void Graph::mouseMoveEvent( QMouseEvent* event ) {
  if ( myMovableItem == true && myMovedItem.isValid() ) {
    myItemPos[ myMovedItem ].setPos( event->pos() );
  }
  MarbAbstractItemView::mouseMoveEvent( event );
}
