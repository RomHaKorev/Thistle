#ifndef THISTLE_POINTCHARTCOORDINATEPLANEVIEW_H
#define THISTLE_POINTCHARTCOORDINATEPLANEVIEW_H

#include "../base/cartesiancoordinateplaneview.h"

namespace Thistle
{

class PointChartCoordinatePlaneView: public CartesianCoordinatePlaneView
{
private:
	void paintGridFor( QPainter& painter, LinearAxis& axis, LinearAxis& other ) const;

protected:
	virtual void paintBack( QPainter& painter ) const;
	virtual void paintGrid( QPainter& painter ) const;
public:
    PointChartCoordinatePlaneView();
    ~PointChartCoordinatePlaneView();

    virtual void update();
};

}
#endif // THISTLE_POINTCHARTCOORDINATEPLANEVIEW_H
