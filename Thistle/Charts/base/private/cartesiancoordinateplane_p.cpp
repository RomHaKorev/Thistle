#include "cartesiancoordinateplane_p.h"

#include "../../../kernel/global.h"

namespace Thistle
{

CartesianCoordinatePlanePrivate::CartesianCoordinatePlanePrivate()
: AbstractCoordinateSystemPrivate()
{
    this->model = 0;
    //this->yAxis().setPrecision( 2 );
    //this->yAxis().setTicksCount( 5 );
	//this->yAxis().setLabelsLength( 10 );
	//this->xAxis().setLabelsLength( 0 );
	this->axis_x.setPrecision( 2 );
	this->axis_x.setLabelsLength( 0 );
	this->axis_y.setTicksCount( 5 );
	this->axis_y.setLabelsLength( 10 );

    this->axisPen = QPen( QColor( Thistle::Colors::DarkGray ), 1 );
    this->verticalLabels = true;
}

CartesianCoordinatePlanePrivate::~CartesianCoordinatePlanePrivate()
{}


const LinearAxis& CartesianCoordinatePlanePrivate::xAxis() const
{
	return this->axis_x;
}

const LinearAxis& CartesianCoordinatePlanePrivate::yAxis() const
{
	return this->axis_y;
}

LinearAxis& CartesianCoordinatePlanePrivate::xAxis()
{
	return this->axis_x;
}

LinearAxis& CartesianCoordinatePlanePrivate::yAxis()
{
	return this->axis_y;
}

AbstractAxis& CartesianCoordinatePlanePrivate::axis( unsigned int index )
{
	if ( index == CartesianCoordinatePlane::X )
		return this->axis_x;
	else
		return this->axis_y;
}


unsigned int CartesianCoordinatePlanePrivate::axisCount() const
{
	return 2;
}


}
