#ifndef GRAPHMODEL_P_H
#define GRAPHMODEL_P_H

#include "edge.h"
#include <QList>

namespace Marb {

struct GraphModelPrivate {
    GraphModelPrivate();
    QList<Edge> edges;
};
}

#endif // GRAPHMODEL_P_H
