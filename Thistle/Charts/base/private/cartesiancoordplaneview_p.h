#ifndef THISTLE_CARTESIANCOORDPLANEVIEW_P_H
#define THISTLE_CARTESIANCOORDPLANEVIEW_P_H

#include <QPen>
#include <QFont>

#include "../../base/private/abstractcoordinatesystemview_p.h"

class QAbstractItemModel;

namespace Thistle
{
class CartesianCoordinatePlane;

struct CartesianCoordPlaneViewPrivate: public AbstractCoordinateSystemViewPrivate
{
    CartesianCoordPlaneViewPrivate( CartesianCoordinatePlane* coordSys );

    CartesianCoordinatePlane* coordinateSystem;

    QRect rect;
    QFont font;
    QPen axisPen;
    QPen tickPen;
    QPen textPen;
    QAbstractItemModel* model;
};

}

#endif // THISTLE_CARTESIANCOORDPLANEVIEW_P_H
