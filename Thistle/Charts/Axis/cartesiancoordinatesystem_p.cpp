#include "cartesiancoordinatesystem_p.h"

#include "../../kernel/global.h"

namespace Thistle
{

CartesianCoordinateSystemPrivate::CartesianCoordinateSystemPrivate()
    : AbstractCoordinateSystemPrivate()
{
    this->axis.append( LinearAxis() );
    this->axis.append( LinearAxis() );

    this->model = 0;
    this->yAxis().setPrecision( 2 );
    this->yAxis().setTicksCount( 5 );

    this->axisPen = QPen( QColor( Thistle::Colors::DarkGray ), 1 );
    this->startOnAxis = false;
    this->verticalLabels = true;

    this->yAxis().setLabelsLength( 10 );
    this->xAxis().setLabelsLength( 0 );
}

}