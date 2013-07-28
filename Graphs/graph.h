#ifndef GRAPH_H
#define GRAPH_H

#include <QTimer>

#include "../marbabstractitemview.h"
#include "node.h"


class Graph : public MarbAbstractItemView {
  Q_OBJECT
protected:
  QList<Edge> myEdges;
  QMap<QModelIndex, Node> myItemPos;
  QTimer myTimer;
  qreal oldLength;
  QSizeF myRealSize;
  QPointF myItemOffset;

  bool myMovableItem;
  bool myElasticItem;
  QModelIndex myMovedItem;

  qreal myWeight;

  virtual void setScrollBarValues();
  QModelIndex indexAt(const QPoint& point) const;

public:
  explicit Graph(QWidget *parent = 0);

    void updateValues();

  virtual QRectF itemRect( const QModelIndex& index ) const;

  void calculateForces( QModelIndex& index );
  void addEdge( QModelIndex idx1, QModelIndex idx2, Edge::Type type = Edge::NoArrow );
  void addEdge( int row1, int col1, int row2, int col2, Edge::Type type = Edge::NoArrow );

  void mousePressEvent( QMouseEvent* event );
  void mouseReleaseEvent( QMouseEvent* event );
  void mouseMoveEvent( QMouseEvent* event );

  void paintEvent( QPaintEvent* event );
  void paintEdges( QPainter& painter, QPointF offset = QPointF( 0, 0 ) );
  void paintItems( QPainter& painter, QPointF offset = QPointF( 0, 0 ) );
  void paintEdge( QPainter& painter, QModelIndex idx1, QModelIndex idx2, Edge::Type type );
  void paintArrow( QPainter& painter, QLineF line );

  void setNodeWeight( qreal weight );

public slots:
  virtual bool save( QString filename );

protected slots:
  void processTimer();
};

#endif // GRAPH_H
