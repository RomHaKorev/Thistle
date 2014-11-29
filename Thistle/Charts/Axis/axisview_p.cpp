#include "axisview_p.h"
#include "cartesiancoordinatesystem.h"
#include "../../kernel/global.h"

#include <QAbstractItemModel>

namespace Thistle
{

AxisViewPrivate::AxisViewPrivate()
    : coordinateSystem( new CartesianCoordinateSystem() )
{
    this->model = 0;
}

}