#ifndef THISTLE_XYAXIS_H
#define THISTLE_XYAXIS_H

#include "orthogonalaxis.h"

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QLineF>
#include <QFont>
#include <QPen>

#include "xyaxis_p.h"

class QAbstractItemModel;

namespace Thistle
{

class XYAxis: public OrthogonalAxis
{
    Q_DECLARE_PRIVATE( XYAxis );

public:
    XYAxis();

    virtual void calculateBounds();

    qreal stepSize() const;
    QPointF valueToPoint( const QPointF& point ) const;

public slots:
    void update();

};

}

#endif // THISTLE_XYAXIS_H
