#include "cartesiancoordinateplaneview_p.h"

#include "../cartesiancoordinateplane.h"

namespace Thistle
{

CartesianCoordinatePlaneViewPrivate::CartesianCoordinatePlaneViewPrivate( AbstractCoordinateSystemView* coordSysView )
: AbstractCoordinateSystemViewPrivate( new CartesianCoordinatePlane(), coordSysView )
{}

CartesianCoordinatePlaneViewPrivate::~CartesianCoordinatePlaneViewPrivate()
{}

CartesianCoordinatePlane* CartesianCoordinatePlaneViewPrivate::coordinateSystem() const
{
    return dynamic_cast<CartesianCoordinatePlane*>( coordSystem );
}

}