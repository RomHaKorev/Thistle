#ifndef THISTLE_CARTESIANCOORDINATEPLANE_P_H
#define THISTLE_CARTESIANCOORDINATEPLANE_P_H

#include <QPen>

#include "../../base/linearaxis.h"
#include "../../base/private/abstractcoordinatesystem_p.h"
#include "../cartesiancoordinateplane.h"

namespace Thistle
{

class LinearAxis;

class CartesianCoordinatePlanePrivate : public AbstractCoordinateSystemPrivate
{
private:
	LinearAxis axis_x;
	LinearAxis axis_y;
public:
	QRectF valuesRect;

    CartesianCoordinatePlanePrivate();
	~CartesianCoordinatePlanePrivate();

	const LinearAxis& xAxis() const;
	const LinearAxis& yAxis() const;

	LinearAxis& xAxis();
	LinearAxis& yAxis();

	virtual AbstractAxis& axis( unsigned int index );
	virtual unsigned int axisCount() const;

    bool verticalLabels;
    QPointF valueOffset;

};

}

#endif // THISTLE_CARTESIANCOORDINATEPLANE_P_H
