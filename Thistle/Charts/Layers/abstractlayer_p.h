#ifndef THISTLE_ABSTRACTLAYER_P_H
#define THISTLE_ABSTRACTLAYER_P_H

#include <QtCore>
#include <QPainter>

namespace Thistle
{
class AbstractChart;
struct AbstractLayerPrivate
{
    AbstractChart* chart;
    qreal zValue;

    AbstractLayerPrivate( AbstractChart* aChart );
    ~AbstractLayerPrivate();

    QAbstractItemModel* model() const;

    void paint( QPainter& painter ) const;
};
}
#endif // THISTLE_ABSTRACTLAYER_P_H
