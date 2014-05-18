#ifndef EDGE_H
#define EDGE_H

#include <QModelIndex>

namespace Thistle {

class Edge {
public:
    enum Type {
        NoArrow = 0,
        Bilateral    = 1,
        Unilateral = 2
    };
    Q_ENUMS( Type )
    Edge( QModelIndex idx1, QModelIndex idx2, Edge::Type type = NoArrow );
    QModelIndex leftIndex;
    QModelIndex rightIndex;
    Edge::Type type;
};

}
#endif // EDGE_H
