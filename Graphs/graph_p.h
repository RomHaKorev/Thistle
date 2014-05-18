#ifndef GRAPH_P_H
#define GRAPH_P_H

#include <QTimer>
#include <QMap>
#include <QPoint>
#include "../kernel/AbstractItemView_p.h"
#include "node.h"
#include "graphmodel.h"

class GraphModel;

namespace Thistle {

class GraphAlgorithm;

struct GraphPrivate : AbstractItemViewPrivate {
    GraphPrivate();
    ~GraphPrivate();

    QSizeF realSize;

    bool movableItem;

    bool elasticItem;

    QModelIndex movedItem;

    QTime dragDropTime;

    qreal weight;

    QPoint itemOffset;

    GraphModel* model;

    GraphAlgorithm* algorithm;
};

}

#endif // GRAPH_P_H
