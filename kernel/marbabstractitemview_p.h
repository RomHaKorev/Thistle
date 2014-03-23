#ifndef MARBABSTRACTITEMVIEW_P_H
#define MARBABSTRACTITEMVIEW_P_H

#include <qpointer.h>
#include <qmap.h>
#include <qrubberband.h>
#include <qmargins.h>

#include "marbitemdelegate.h"

class MarbAbstractItemView;

struct MarbAbstractItemViewPrivate {
    QMap<QModelIndex, QPointF> itemPos;
    QPointer<MarbItemDelegate> delegate;
    QRubberBand rubberBand;
    MarbAbstractItemView* p;
    QMargins margins;
    MarbAbstractItemViewPrivate( MarbAbstractItemView* parent );
};

#endif // MARBABSTRACTITEMVIEW_P_H
