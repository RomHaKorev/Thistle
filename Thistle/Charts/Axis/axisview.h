#ifndef THISTLE_CARTESIANAXISVIEW_H
#define THISTLE_CARTESIANAXISVIEW_H

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QLineF>
#include <QFont>
#include <QPen>

#include "axisview_p.h"
#include "cartesiancoordinatesystem.h"

class QAbstractItemModel;

namespace Thistle
{
class CartesianCoordinateSystem;

class AxisView
{
    Q_DECLARE_PRIVATE( AxisView );
protected:
    AxisViewPrivate* d_ptr;
    //CartesianAxisView( CartesianCoordinateSystemPrivate* d );

    void paintXAxis( QPainter& painter ) const;
    void paintYAxis( QPainter& painter ) const;

public:

    AxisView();

    qreal labelsLength( CartesianCoordinateSystem::Axis axis ) const;
    int   ticksCount( CartesianCoordinateSystem::Axis axis ) const;
    long  order( CartesianCoordinateSystem::Axis axis ) const;
    qreal tickSize( CartesianCoordinateSystem::Axis axis ) const;

    void setLabelsLength( CartesianCoordinateSystem::Axis axis, int length );
    void setTicksCount( CartesianCoordinateSystem::Axis axis, int nb );
    void setOrder( CartesianCoordinateSystem::Axis axis, long order );
    void setTickSize( CartesianCoordinateSystem::Axis axis, qreal size );
     
    QFont font() const;
    void setFont( const QFont& );

    void paintBack( QPainter& painter ) const;
    void paintFront( QPainter& painter ) const;

    qreal stepSize() const;

    QPointF origin() const;

    void setRect( const QRect& );

    CartesianCoordinateSystem* coordinateSystem() const;

    void setModel( QAbstractItemModel* model );
    QAbstractItemModel* model() const;

public slots:
    void update();

};

}

#endif // THISTLE_CARTESIANAXISVIEW_H