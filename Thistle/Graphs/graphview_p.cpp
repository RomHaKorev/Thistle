#include "graphview_p.h"
#include "abstractgraphalgorithm.h"
namespace Thistle
{

GraphViewPrivate::GraphViewPrivate()
{
    model = 0;
    itemOffset = QPoint( 0, 0 );
}

GraphViewPrivate::~GraphViewPrivate()
{
    algorithm->deleteLater();
}

}