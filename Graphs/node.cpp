#include "node.h"

bool operator==( Edge& e1, Edge& e2 ) {
  if ( e1.leftIndex == e2.leftIndex && e1.rightIndex == e2.rightIndex ) {
    return true;
  }
}

Edge::Edge( QModelIndex idx1, QModelIndex idx2, Edge::Type t ) {
  leftIndex = idx1;
  rightIndex = idx2;
  type = t;
}

Node::Node() : myPos(0, 0) {

}

QPointF Node::pos() const {
  return myPos;
}

void Node::setPos( qreal x, qreal y ) {
  setPos( QPointF( x, y ) );
}

void Node::setPos( QPointF p ) {
  myPos = p;
}

void Node::process() {

}

void Node::addEdge( Node* node ) {
  if ( node != this ) {
    myConnectedNode << node;
  }
}

QList<Node*> Node::edges() const {
  return myConnectedNode;
}
