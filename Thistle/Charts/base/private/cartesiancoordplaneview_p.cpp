#include "cartesiancoordplaneview_p.h"
#include "../cartesiancoordinateplane.h"
#include "../../kernel/global.h"

#include <QAbstractItemModel>

namespace Thistle
{

CartesianCoordPlaneViewPrivate::CartesianCoordPlaneViewPrivate( CartesianCoordinatePlane* coordSys )
:  AbstractCoordSysViewPrivate( coordSys  ), coordinateSystem( coordSys )
{
    
    this->model = 0;
}

}