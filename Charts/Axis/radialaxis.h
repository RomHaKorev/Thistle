#ifndef RADIALAXIS_H
#define RADIALAXIS_H
#include "axis.h"
#include <QPointF>



class RadialAxis : public Axis {
protected:
    qreal myCenterHoleDiam;
    QPointF myOrigin;
    QLineF yAxis;

public:
    RadialAxis();
    ~RadialAxis();
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

#endif // RADIALAXIS_H