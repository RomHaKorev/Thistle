#ifndef GRAPHALGORITHM_H
#define GRAPHALGORITHM_H

#include <QObject>
#include <QTimer>

#include "edge.h"
#include "node.h"

#include "graphalgorithm_p.h"
#include "abstractgraphalgorithm.h"

namespace Thistle
{

class GraphView;

class GraphAlgorithm : public AbstractGraphAlgorithm
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( GraphAlgorithm );
private:
    void calculateForces( const QModelIndex& index );
public:
    GraphAlgorithm( GraphView* parent );

    void run();

    inline void setPosition( const QModelIndex& index, QPointF pos )
    {
        Q_D( GraphAlgorithm );
        d->itemPos[ index ].setPos( pos );
    }
    void setNodeWeight( qreal weight );
private slots:
    void processTimer();
};

}

#endif // GRAPHALGORITHM_H
