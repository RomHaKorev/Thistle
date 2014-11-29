#ifndef THISTLE_RADARLCOORDINATESYSTEM_H
#define THISTLE_RADARLCOORDINATESYSTEM_H

#include "abstractcoordinatesystem.h"
#include "radarcoordinatesystem_p.h"

#include <QPointF>

namespace Thistle
{

class RadarCoordinateSystem : public AbstractCoordinateSystem
{
    Q_DECLARE_PRIVATE( RadarCoordinateSystem );
public:
    RadarCoordinateSystem();
    ~RadarCoordinateSystem();
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
#endif
private:
    void paintText( QPainter& painter ) const;
};

}

#endif // THISTLE_RADARLCOORDINATESYSTEM_H
