#ifndef GRAPHALGORITHM_P_H
#define GRAPHALGORITHM_P_H

#include <QTimer>
#include <QTime>
#include <QSizeF>
#include <QPointF>

#include "node.h"

namespace Marb {


struct GraphAlgorithmPrivate {
    QMap<QModelIndex, Node> itemPos;
    QTimer* timer;
    qreal oldLength;
    QSizeF realSize;

    bool movableItem;
    bool elasticItem;
    QModelIndex movedItem;
    QTime dragDropTime;

    qreal weight;

    QPoint itemOffset;

    GraphAlgorithmPrivate();
    ~GraphAlgorithmPrivate();
    
};

}

#endif // GRAPHALGORITHM_P_H
