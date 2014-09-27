#include "graphalgorithm_p.h"

namespace Thistle
{

GraphAlgorithmPrivate::GraphAlgorithmPrivate()
    : timer( new QTimer() )
{
    timer->setInterval( 10 );
    movableItem = true;
    elasticItem = true;
    weight = 9.81;
    itemOffset = QPoint( 0, 0 );
}

GraphAlgorithmPrivate::~GraphAlgorithmPrivate()
{
    this->timer->deleteLater();
}

}
