#ifndef THISTLE_ORTHOGONALAXIS_P_H
#define THISTLE_ORTHOGONALAXIS_P_H

#include <QPen>
#include "linearaxis.h"

#include "abstractcoordinatesystem_p.h"

namespace Thistle
{

struct CartesianCoordinateSystemPrivate : public AbstractCoordinateSystemPrivate
{
    CartesianCoordinateSystemPrivate();

    inline const LinearAxis& xAxis() const
    {
        return this->axis.at(0);
    }
    inline const LinearAxis& yAxis() const
    {
        return this->axis.at(1);
    }

    inline LinearAxis& xAxis()
    {
        return this->axis[0];
    }
    inline LinearAxis& yAxis()
    {
        return this->axis[1];
    }

    bool startOnAxis;
    bool verticalLabels;
};

}

#endif // THISTLE_ORTHOGONALAXIS_P_H
