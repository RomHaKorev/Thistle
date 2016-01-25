#include "cartesiancoordplaneview_p.h"
#include "../cartesiancoordinateplane.h"
#include "../../../Core/global.h"

#include <QAbstractItemModel>

namespace Thistle
{

CartesianCoordPlaneViewPrivate::CartesianCoordPlaneViewPrivate( CartesianCoordinatePlane* coordSys )
:  AbstractCoordinateSystemViewPrivate( coordSys  ), coordinateSystem( coordSys )
{

    this->model = 0;
}

}
