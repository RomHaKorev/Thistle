#ifndef RADIALTREELAYOUT_P_H
#define RADIALTREELAYOUT_P_H

#include "treelayout_p.h"

namespace Thistle
{

struct RadialTreeLayoutPrivate : public TreeLayoutPrivate
{
	RadialTreeLayoutPrivate();
	virtual ~RadialTreeLayoutPrivate();
	void updatePerimeter();

	virtual void calculateTreePositions();
	virtual void calculateViewPositions();

	virtual QRect itemRect( const QModelIndex& index = QModelIndex() ) const;

	virtual QPointF scan( const QModelIndex& index, const QPointF& leftDepth = QPointF( 0, 0 ) );

	qreal diagonal;
	qreal perimeter;
	qreal radius;
	QList<QModelIndex> orderedIndexes;
	qreal maxRad;
	QPointF currentCenter;
};

}

#endif // RADIALTREE_P_H
