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
class GraphView;

struct GraphViewPrivate : AbstractItemViewPrivate
{
    GraphViewPrivate( GraphView* view );
    ~GraphViewPrivate();

    QRectF boundingRect;
    QPoint itemOffset;
    GraphModel* model;
    AbstractGraphAlgorithm* algorithm;

    QModelIndex findItemAt( const QPointF& pos ) const;
};

}

#endif // GRAPH_P_H
