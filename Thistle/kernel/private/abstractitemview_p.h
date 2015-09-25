#ifndef ABSTRACTITEMVIEW_P_H
#define ABSTRACTITEMVIEW_P_H

#include <qpointer.h>
#include <qmap.h>
#include <qrubberband.h>
#include <qmargins.h>
#include <qobject.h>

#include "../itemdelegate.h"

namespace Thistle
{

class AbstractItemView;

struct AbstractItemViewPrivate : public QObject
{
	inline AbstractItemView* q_func()
	{
		return q_ptr;
	}
	AbstractItemView* q_ptr;

	QMap<QModelIndex, QPointF> itemPos;
	ItemDelegate* delegate;
	QRubberBand* rubberBand;

	QMargins margins;
	AbstractItemViewPrivate( AbstractItemView* parent = 0 );
	virtual ~AbstractItemViewPrivate();

	virtual QModelIndex findItemAt( const QPointF& pos ) const;
};

}
#endif // ABSTRACTITEMVIEW_P_H
