#ifndef NODE_H
#define NODE_H

#include <QPointF>
#include <QList>
#include <QModelIndex>

class Edge {
public:
  enum Type {
    NoArrow = 0,
    Bilateral  = 1,
    Unilateral = 2
  };
  Q_ENUMS( Type )
  Edge( QModelIndex idx1, QModelIndex idx2, Edge::Type type = NoArrow );
  QModelIndex leftIndex;
  QModelIndex rightIndex;
  Edge::Type type;
};

class Node {
protected:
  QPointF myPos;
  QPointF myNewPos;
  QList<Node*> myConnectedNode;

public:
  Node();
  void process();
  void calculateForce();
  QPointF pos() const;
  QList<Node*> edges() const;
  void setPos( qreal x, qreal y );
  void setPos( QPointF p );
  void addEdge( Node* node );

};

#endif // NODE_H
