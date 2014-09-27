#ifndef ABSTRACTITEMVIEW_P_H
#define ABSTRACTITEMVIEW_P_H

#include <qpointer.h>
#include <qmap.h>
#include <qrubberband.h>
#include <qmargins.h>
#include <qobject.h>

#include "itemdelegate.h"

namespace Thistle
{

class AbstractItemView;

struct AbstractItemViewPrivate : public QObject
{
    QMap<QModelIndex, QPointF> itemPos;
    ItemDelegate* delegate;
    QRubberBand* rubberBand;

    QMargins margins;
    AbstractItemViewPrivate( AbstractItemView* parent = 0 );
    ~AbstractItemViewPrivate();
};

}
#endif // ABSTRACTITEMVIEW_P_H
