#ifndef ABSTRACTTREE_P_H
#define ABSTRACTTREE_P_H

#include <QPoint>
#include <QSize>
#include <QPointer>
#include <QMap>

#include "../kernel/itemdelegate.h"
#include "../kernel/abstractitemview_p.h"

namespace Thistle {

struct AbstractTreePrivate : public AbstractItemViewPrivate {
    AbstractTreePrivate();

    QPen connectionPen;

    QSize realSize;
    QRect itemRect;
    int yDistance;
    int xDistance;
    qreal depth;
    qreal left;
    QPoint centralItemPos;
    QPointF itemOffset;
    QPointer<ItemDelegate> delegate;
    QMap<QModelIndex, QPointF> itemPos;
    QMap<QModelIndex, QPointF> itemTreePos;
};

}

#endif // ABSTRACTTREE_P_H
