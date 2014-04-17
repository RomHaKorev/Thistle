#ifndef RADIALTREE_P_H
#define RADIALTREE_P_H

#include "tree_p.h"

struct RadialTreePrivate : public TreePrivate {
    RadialTreePrivate();

    qreal diagonal;
    qreal perimeter;
    qreal radius;
    QList<QModelIndex> orderedIndexes;
    qreal rotateText;
    qreal maxRad;
    QPointF currentCenter;
};

#endif // RADIALTREE_P_H
