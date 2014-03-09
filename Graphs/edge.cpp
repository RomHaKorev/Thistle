#include "edge.h"

bool operator==( Edge& e1, Edge& e2 ) {
    if ( e1.leftIndex == e2.leftIndex && e1.rightIndex == e2.rightIndex ) {
        return true;
    }
    return false;
}

Edge::Edge( QModelIndex idx1, QModelIndex idx2, Edge::Type t ) {
    leftIndex = idx1;
    rightIndex = idx2;
    type = t;
}
