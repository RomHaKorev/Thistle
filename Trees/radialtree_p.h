#ifndef RADIALTREE_P_H
#define RADIALTREE_P_H

#include "abstracttree_p.h"

namespace Thistle {

struct RadialTreePrivate : public AbstractTreePrivate {
    RadialTreePrivate();

    qreal diagonal;
    qreal perimeter;
    qreal radius;
    QList<QModelIndex> orderedIndexes;
    qreal rotateText;
    qreal maxRad;
    QPointF currentCenter;
};

}

#endif // RADIALTREE_P_H
