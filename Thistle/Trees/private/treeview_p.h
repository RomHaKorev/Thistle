#ifndef THISTLE_TREEVIEW_P_H
#define THISTLE_TREEVIEW_P_H

#include <QPoint>
#include <QSize>
#include <QPointer>
#include <QMap>

#include "../../Core/itemdelegate.h"
#include "../../Core/private/abstractitemview_p.h"

#include "../treeLayout.h"

namespace Thistle
{

struct TreeViewPrivate : public AbstractItemViewPrivate
{
	TreeViewPrivate();
	virtual ~TreeViewPrivate();

	QPointer<TreeLayout> layout;

	QPen connectionPen;

	QPoint centralItemPos;
	QPointF itemOffset;
	QPointer<ItemDelegate> delegate;
};

}

#endif // THISTLE_TREEVIEW_P_H
