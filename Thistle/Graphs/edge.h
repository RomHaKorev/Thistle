#ifndef EDGE_H
#define EDGE_H

#include <QModelIndex>
#include <QPainter>

namespace Thistle
{
class Edge
{
public:
    enum Type
    {
        NoArrow = 0,
        Bilateral    = 1,
        Unilateral = 2
    };
    Q_ENUMS( Type );

    Edge( QModelIndex idx1, QModelIndex idx2, Edge::Type type = NoArrow );

    QModelIndex leftIndex;
    QModelIndex rightIndex;
    Edge::Type type;

    void paintEdge( QPainter& painter, QRectF r1, QRectF r2 ) const;

    void paintArrow( QPainter& painter, const QLineF& line ) const;
};

}
#endif // EDGE_H
