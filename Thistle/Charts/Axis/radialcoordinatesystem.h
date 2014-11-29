#ifndef THISTLE_RADIALCOORDINATESYSTEM_H
#define THISTLE_RADIALCOORDINATESYSTEM_H
#include "abstractcoordinatesystem.h"
#include "radialcoordinatesystem_p.h"

#include <QPointF>

namespace Thistle
{

class RadialCoordinateSystem : public AbstractCoordinateSystem
{
    Q_DECLARE_PRIVATE( RadialCoordinateSystem );
public:
    RadialCoordinateSystem();
    ~RadialCoordinateSystem();
#if 0
    qreal centerHoleDiam() const;
    virtual QPointF origin() const;
    virtual void update();
    virtual QPointF valueToPoint( qreal value, int axisNumber ) const;
    virtual QRect valueToRect( qreal value ) const;
    virtual void paintBack( QPainter& painter ) const;
    virtual void paintFront( QPainter& painter ) const;
    qreal startAngle() const;
    virtual qreal stepSize() const;

private:
    void paintText( QPainter& painter ) const;
#endif
};

}

#endif // THISTLE_RADIALCOORDINATESYSTEM_H
