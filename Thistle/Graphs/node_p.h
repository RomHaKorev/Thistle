#ifndef NODE_P_H
#define NODE_P_H

#include <QPointF>
#include <QList>

namespace Thistle
{

class Node;

struct NodePrivate
{
    QPointF pos;
    QPointF newPos;
    QList<Node*> connectedNode;
    NodePrivate();
    ~NodePrivate();
};

}

#endif //NODE_P_H