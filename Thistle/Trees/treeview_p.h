#ifndef TREEVIEW_P_H
#define TREEVIEW_P_H

#include <QPoint>
#include <QSize>
#include <QPointer>
#include <QMap>

#include "../kernel/itemdelegate.h"
#include "../kernel/abstractitemview_p.h"

#include "treeLayout.h"

namespace Thistle
{

struct TreeViewPrivate : public AbstractItemViewPrivate
{
    TreeViewPrivate();

    QPointer<TreeLayout> layout;

    QPen connectionPen;

    QPoint centralItemPos;
    QPointF itemOffset;
    QPointer<ItemDelegate> delegate;
};

}

#endif // TREEVIEW_P_H
