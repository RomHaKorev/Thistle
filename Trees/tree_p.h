#ifndef TREE_P_H
#define TREE_P_H

#include <QPoint>
#include <QSize>
#include <QPointer>
#include <QMap>

#include "../kernel/itemdelegate.h"
#include "../kernel/abstractitemview_p.h"

struct TreePrivate : public AbstractItemViewPrivate {
    TreePrivate();

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

#endif // TREE_P_H
