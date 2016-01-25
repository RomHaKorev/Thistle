#ifndef THISTLE_LINECHARTCOORDINATEPLANEVIEW_H
#define THISTLE_LINECHARTCOORDINATEPLANEVIEW_H

#include "../base/cartesiancoordinateplaneview.h"

namespace Thistle
{

class LineChartCoordinatePlaneView: public CartesianCoordinatePlaneView
{
protected:
	virtual void paintFront( QPainter& painter ) const;
	virtual void paintBack( QPainter& painter ) const;

public:
    LineChartCoordinatePlaneView();
    ~LineChartCoordinatePlaneView();

public slots:
	virtual void update();
};

}
#endif // THISTLE_LINECHARTCOORDINATEPLANEVIEW_H
