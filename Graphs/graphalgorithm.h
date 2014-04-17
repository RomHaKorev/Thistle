#ifndef GRAPHALGORITHM_H
#define GRAPHALGORITHM_H

#include <QObject>
#include <QTimer>

#include "edge.h"
#include "node.h"

#include "graphalgorithm_p.h"

namespace Marb {

class Graph;

class GraphAlgorithm : public QObject {
    Q_OBJECT
private:
    void calculateForces( const QModelIndex& index );
    GraphAlgorithmPrivate* d_ptr;
public:
    GraphAlgorithm( Graph* parent );

    void run();

    const Node node( const QModelIndex& index ) const;
    inline void setPosition( const QModelIndex& index, QPointF pos ) { d_ptr->itemPos[ index ].setPos( pos ); }
private slots:
    void processTimer();

signals:
    void update();
};

}

#endif // GRAPHALGORITHM_H
