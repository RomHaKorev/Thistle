#ifndef THISTLE_GRAPHMODEL_P_H
#define THISTLE_GRAPHMODEL_P_H

#include "../edge.h"
#include <QList>

namespace Thistle
{

struct GraphModelPrivate
{
	GraphModelPrivate();
	QList<Thistle::Edge> edges;
};
}

#endif // THISTLE_GRAPHMODEL_P_H
