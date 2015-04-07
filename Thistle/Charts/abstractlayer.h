#ifndef THISTLE_ABSTRACTLAYER_H
#define THISTLE_ABSTRACTLAYER_H

#include <QObject>
#include <QPainter>

namespace Thistle
{
class AbstractChart;
class AbstractLayerPrivate;

class AbstractLayer : public QObject
{
    Q_OBJECT
friend class AbstractChart;

protected:
    AbstractLayerPrivate* d_ptr;

public:
    AbstractLayer( AbstractChart* parent );
    ~AbstractLayer();

    qreal zValue() const;
    void setZValue( qreal value );

    void paint( QPainter& painter );
};
}
#endif // THISTLE_ABSTRACTLAYER_H
