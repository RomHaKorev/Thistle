#ifndef THISTLE_CARTESIANCOORDINATESYSTEM_H
#define THISTLE_CARTESIANCOORDINATESYSTEM_H

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QLineF>
#include <QFont>
#include <QPen>

#include "abstractcoordinatesystem.h"

namespace Thistle
{
class CartesianCoordinateSystemPrivate;

class CartesianCoordinateSystem : public AbstractCoordinateSystem
{
    Q_DECLARE_PRIVATE( CartesianCoordinateSystem );
protected:
    CartesianCoordinateSystem( CartesianCoordinateSystemPrivate* d );

    void paintXAxis( QPainter& painter ) const;
    void paintYAxis( QPainter& painter ) const;

public:

    enum Axis
    {
        Y = 0,
        X = 1
    };

    CartesianCoordinateSystem();

    qreal labelsLength( CartesianCoordinateSystem::Axis axis ) const;
    qreal maximum( CartesianCoordinateSystem::Axis axis ) const;
    qreal minimum( CartesianCoordinateSystem::Axis axis ) const;
    int   precision( CartesianCoordinateSystem::Axis axis ) const;
    int   ticksCount( CartesianCoordinateSystem::Axis axis ) const;
    long  order( CartesianCoordinateSystem::Axis axis ) const;
    qreal tickSize( CartesianCoordinateSystem::Axis axis ) const;

    void setLabelsLength( CartesianCoordinateSystem::Axis axis, int length );
    void setPrecision( CartesianCoordinateSystem::Axis axis, int nb );
    void setTicksCount( CartesianCoordinateSystem::Axis axis, int nb );
    void setOrder( CartesianCoordinateSystem::Axis axis, long order );
    void setTickSize( CartesianCoordinateSystem::Axis axis, qreal size );
    void setBounds( CartesianCoordinateSystem::Axis axis, qreal minimum, qreal maximum );
    void setAxisLine( CartesianCoordinateSystem::Axis axis, const QLineF& line );
    const QLineF axisLine(CartesianCoordinateSystem::Axis axis );
     
    static qreal calculateOrder( qreal value );
    QFont font() const;
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

#endif // THISTLE_CARTESIANCOORDINATESYSTEM_H
