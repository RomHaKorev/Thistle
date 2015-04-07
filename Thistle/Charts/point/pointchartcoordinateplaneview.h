#ifndef THISTLE_POINTCHARTCOORDINATEPLANEVIEW_H
#define THISTLE_POINTCHARTCOORDINATEPLANEVIEW_H

#include "../base/cartesiancoordinateplaneview.h"

namespace Thistle
{

class PointChartCoordinatePlaneView: public CartesianCoordinatePlaneView
{
public:
    PointChartCoordinatePlaneView();
    ~PointChartCoordinatePlaneView();

    virtual void update();
};

}
#endif // THISTLE_POINTCHARTCOORDINATEPLANEVIEW_H
