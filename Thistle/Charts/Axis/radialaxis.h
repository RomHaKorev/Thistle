#ifndef RADIALAXIS_H
#define RADIALAXIS_H
#include "abstractaxis.h"
#include <QPointF>

#include "radialaxis_p.h"

namespace Thistle {

    class RadialAxis : public AbstractAxis {
        Q_DECLARE_PRIVATE( RadialAxis );
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

}

#endif // RADIALAXIS_H