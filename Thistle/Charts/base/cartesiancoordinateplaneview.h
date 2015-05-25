#ifndef THISTLE_CARTESIANCOORDPLANEVIEW_H
#define THISTLE_CARTESIANCOORDPLANEVIEW_H

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QLineF>
#include <QFont>
#include <QPen>

#include "private/cartesiancoordinateplaneview_p.h"
#include "cartesiancoordinateplane.h"
#include "../base/abstractcoordinatesystem.h"
#include "../base/abstractcoordinatesystemview.h"
#include "linearaxisdelegate.h"

class QAbstractItemModel;

namespace Thistle
{
class CartesianCoordinatePlaneViewPrivate;


class CartesianCoordinatePlaneView: public AbstractCoordinateSystemView
{
protected:
    Q_DECLARE_PRIVATE( CartesianCoordinatePlaneView );

public:

    CartesianCoordinatePlaneView();

    qreal labelsLength( CartesianCoordinatePlane::Axis axis ) const;
    int   ticksCount( CartesianCoordinatePlane::Axis axis ) const;
    long  order( CartesianCoordinatePlane::Axis axis ) const;

    void setLabelsLength( CartesianCoordinatePlane::Axis axis, int length );
    void setTicksCount( CartesianCoordinatePlane::Axis axis, int nb );
    void setOrder( CartesianCoordinatePlane::Axis axis, long order );
     
    virtual void paintBack( QPainter& painter ) const;
    virtual void paintFront( QPainter& painter ) const;

    QPointF origin() const;

    void setRect( const QRect& );

    CartesianCoordinatePlane* coordinateSystem() const;

    virtual void update() = 0;

};

}

#endif // THISTLE_CARTESIANCOORDPLANEVIEW_H