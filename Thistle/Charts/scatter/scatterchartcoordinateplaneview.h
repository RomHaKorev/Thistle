#ifndef THISTLE_SCATTERCHARTCOORDINATEPLANEVIEW_H
#define THISTLE_SCATTERCHARTCOORDINATEPLANEVIEW_H

#include "../base/cartesiancoordinateplaneview.h"

namespace Thistle
{

class ScatterChartCoordinatePlaneView: public CartesianCoordinatePlaneView
{
protected:
	virtual void paintBack( QPainter& painter ) const;

public:
	ScatterChartCoordinatePlaneView();
	~ScatterChartCoordinatePlaneView();

	virtual void update();
};

}
#endif // THISTLE_SCATTERCHARTCOORDINATEPLANEVIEW_H
