#ifndef ABSTRACTITEMVIEW_P_H
#define ABSTRACTITEMVIEW_P_H

#include <qpointer.h>
#include <qmap.h>
#include <qrubberband.h>
#include <qmargins.h>

#include "itemdelegate.h"

class AbstractItemView;

struct AbstractItemViewPrivate {
    QMap<QModelIndex, QPointF> itemPos;
    ItemDelegate* delegate;
    QRubberBand* rubberBand;

    QMargins margins;
    AbstractItemViewPrivate( AbstractItemView* parent = 0 );
    ~AbstractItemViewPrivate();
};

#endif // ABSTRACTITEMVIEW_P_H
