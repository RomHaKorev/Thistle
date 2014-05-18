#ifndef ORTHOGONALAXIS_H
#define ORTHOGONALAXIS_H

#include "axis.h"

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QLineF>
#include <QFont>
#include <QPen>

class QAbstractItemModel;

namespace Thistle {
class OrthogonalAxisPrivate;

class OrthogonalAxis : public Axis {
    Q_DECLARE_PRIVATE( OrthogonalAxis )
protected:
    void paintXAxis( QPainter& painter ) const;
    void paintYAxis( QPainter& painter ) const;

public:
    OrthogonalAxis();

    void calculateBounds();
    static qreal calculateOrder( qreal value );
    QFont font() const;
    QAbstractItemModel* model() const;
    virtual QPointF origin() const;

    void paintBack( QPainter& painter ) const;
    void paintFront( QPainter& painter ) const;

    void setstartOnAxis( bool startOn );
    bool startOnAxis() const;
    qreal stepSize() const;
    QPointF valueToPoint( qreal value, int axisNumber ) const;

public slots:
    void update();

};

}

#endif // ORTHOGONALAXIS_H
