#ifndef CARTESIANCOORDINATEPLANEVIEW_P_H
#define CARTESIANCOORDINATEPLANEVIEW_P_H

#include "../../base/private/abstractcoordinatesystemview_p.h"

#include "../../base/abstractcoordinatesystem.h"
#include "../cartesiancoordinateplane.h"

namespace Thistle
{
class CartesianCoordinatePlane;

struct CartesianCoordinatePlaneViewPrivate : public AbstractCoordinateSystemViewPrivate
{
	CartesianCoordinatePlane* coordinateSystem() const;

	CartesianCoordinatePlaneViewPrivate( AbstractCoordinateSystemView* coordSysView );
	virtual ~CartesianCoordinatePlaneViewPrivate();
};

}
#endif // CARTESIANCOORDINATEPLANEVIEW_P_H
