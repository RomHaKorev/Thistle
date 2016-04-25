#ifndef THISTLE_GRAPHVIEW_P_H
#define THISTLE_GRAPHVIEW_P_H

#include <QTimer>
#include <QMap>
#include <QPoint>
#include "../../Core/private/abstractitemview_p.h"
#include "../node.h"
#include "../graphmodel.h"

class GraphModel;

namespace Thistle
{

class AbstractGraphAlgorithm;
class GraphView;

struct GraphViewPrivate : AbstractItemViewPrivate
{
	GraphViewPrivate( GraphView* view );
	virtual ~GraphViewPrivate();

	QRectF boundingRect;
	QPoint itemOffset;
	GraphModel* model;
	AbstractGraphAlgorithm* algorithm;

	QModelIndex findItemAt( const QPointF& pos ) const;
};

}

#endif // THISTLE_GRAPHVIEW_P_H
