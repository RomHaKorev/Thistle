#include "graph_p.h"
#include "graphalgorithm.h"
namespace Thistle {

GraphPrivate::GraphPrivate() {
    movableItem = true;
    elasticItem = true;
    weight = 9.81;
    model = 0;
    itemOffset = QPoint( 0, 0 );
}

GraphPrivate::~GraphPrivate() {
    algorithm->deleteLater();
}

}