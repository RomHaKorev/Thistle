#ifndef RADARLAXIS_H
#define RADARLAXIS_H

#include "abstractaxis.h"
#include <QPointF>

#include "radaraxis_p.h"

namespace Thistle
{

class RadarAxis : public AbstractAxis
{
    Q_DECLARE_PRIVATE( RadarAxis );
public:
    RadarAxis();
    ~RadarAxis();
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
};

}

#endif // RADARLAXIS_H