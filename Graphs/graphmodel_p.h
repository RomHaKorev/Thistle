#ifndef GRAPHMODEL_P_H
#define GRAPHMODEL_P_H

#include "edge.h"
#include <QList>

//namespace Thistle {

struct GraphModelPrivate {
    GraphModelPrivate();
    QList<Thistle::Edge> edges;
};
//}

#endif // GRAPHMODEL_P_H
