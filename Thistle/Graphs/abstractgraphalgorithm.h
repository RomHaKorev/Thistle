#ifndef ABSTRACTGRAPHALGORITHM_H
#define ABSTRACTGRAPHALGORITHM_H
#include <QObject>

#include "edge.h"
#include "node.h"
#include "abstractgraphalgorithm_p.h"

namespace Thistle
{

class GraphView;

class AbstractGraphAlgorithm : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( AbstractGraphAlgorithm );
protected:
    AbstractGraphAlgorithmPrivate* d_ptr;
    AbstractGraphAlgorithm( AbstractGraphAlgorithmPrivate* d, GraphView* parent );
public:
    AbstractGraphAlgorithm( GraphView* parent );
    ~AbstractGraphAlgorithm();

    virtual void run() = 0;

    const Node node( const QModelIndex& index ) const;
    inline void setPosition( const QModelIndex& index, QPointF pos )
    {
        d_ptr->itemPos[ index ].setPos( pos );
    }
signals:
    void updated();
};

}

#endif // ABSTRACTGRAPHALGORITHM_H
