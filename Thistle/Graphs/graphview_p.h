#ifndef GRAPH_P_H
#define GRAPH_P_H

#include <QTimer>
#include <QMap>
#include <QPoint>
#include "../kernel/AbstractItemView_p.h"
#include "node.h"
#include "graphmodel.h"

class GraphModel;

namespace Thistle
{

class AbstractGraphAlgorithm;

struct GraphViewPrivate : AbstractItemViewPrivate
{
    GraphViewPrivate();
    ~GraphViewPrivate();

    QRectF boundingRect;
    QPoint itemOffset;
    GraphModel* model;
    AbstractGraphAlgorithm* algorithm;
};

}

#endif // GRAPH_P_H
