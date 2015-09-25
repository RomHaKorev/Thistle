#ifndef THISTLE_ABSTRACTGRAPHALGORITHM_P_H
#define THISTLE_ABSTRACTGRAPHALGORITHM_P_H

#include <QModelIndex>
#include <QMap>

#include "../node.h"


namespace Thistle
{

struct AbstractGraphAlgorithmPrivate
{
	QMap<QModelIndex, Node> itemPos;

	AbstractGraphAlgorithmPrivate();
	virtual ~AbstractGraphAlgorithmPrivate();
};

}

#endif // THISTLE_ABSTRACTGRAPHALGORITHM_P_H
