#ifndef THISTLE_CARTESIANCOORDINATEPLANE_H
#define THISTLE_CARTESIANCOORDINATEPLANE_H

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QLineF>
#include <QFont>
#include <QPen>

#include "../base/abstractcoordinatesystem.h"

namespace Thistle
{
class CartesianCoordinatePlanePrivate;

class CartesianCoordinatePlane : public AbstractCoordinateSystem
{
    Q_DECLARE_PRIVATE( CartesianCoordinatePlane );
protected:
    CartesianCoordinatePlane( CartesianCoordinatePlanePrivate* d );

    void paintXAxis( QPainter& painter ) const;
    void paintYAxis( QPainter& painter ) const;

public:

    enum Axis
    {
        Y = 0,
        X = 1
    };

    CartesianCoordinatePlane();

    qreal labelsLength( CartesianCoordinatePlane::Axis axis ) const;
    qreal maximum( CartesianCoordinatePlane::Axis axis ) const;
    qreal minimum( CartesianCoordinatePlane::Axis axis ) const;
    
    int   ticksCount( CartesianCoordinatePlane::Axis axis ) const;
    long  order( CartesianCoordinatePlane::Axis axis ) const;
    qreal tickSize( CartesianCoordinatePlane::Axis axis ) const;

    void setLabelsLength( CartesianCoordinatePlane::Axis axis, int length );
    void setTicksCount( CartesianCoordinatePlane::Axis axis, int nb );
    void setOrder( CartesianCoordinatePlane::Axis axis, long order );
    void setTickSize( CartesianCoordinatePlane::Axis axis, qreal size );
    void setBounds( CartesianCoordinatePlane::Axis axis, qreal minimum, qreal maximum );
    void setAxisLine( CartesianCoordinatePlane::Axis axis, const QLineF& line );
    const QLineF& axisLine(CartesianCoordinatePlane::Axis axis );
     
    static qreal calculateOrder( qreal value );
    virtual QPointF origin() const;

    void setStartOnAxis( bool startOn );
    bool startOnAxis() const;
    qreal stepSize() const;
    QPointF valueToPoint( qreal x, qreal y ) const;
    virtual void update( QAbstractItemModel* model = 0 );

};

}

#endif // THISTLE_CARTESIANCOORDINATEPLANE_H
