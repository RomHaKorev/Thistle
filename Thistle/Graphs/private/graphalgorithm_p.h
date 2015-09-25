#ifndef THISTLE_GRAPHALGORITHM_P_H
#define THISTLE_GRAPHALGORITHM_P_H

#include <QTimer>
#include <QTime>
#include <QSizeF>
#include <QPointF>

#include "../node.h"
#include "abstractgraphalgorithm_p.h"

namespace Thistle
{
struct GraphAlgorithmPrivate : public AbstractGraphAlgorithmPrivate
{
	QTimer* timer;
	qreal oldLength;
	QSizeF realSize;

	bool movableItem;
	bool elasticItem;
	QModelIndex movedItem;
	QTime dragDropTime;

	qreal weight;

	QPoint itemOffset;

	GraphAlgorithmPrivate();
	virtual ~GraphAlgorithmPrivate();

};

}

#endif // THISTLE_GRAPHALGORITHM_P_H
