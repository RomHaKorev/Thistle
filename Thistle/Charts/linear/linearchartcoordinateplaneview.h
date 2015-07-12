#ifndef THISTLE_LINEARCHARTCOORDINATEPLANEVIEW_H
#define THISTLE_LINEARCHARTCOORDINATEPLANEVIEW_H

#include "../base/cartesiancoordinateplaneview.h"

namespace Thistle
{

class LinearChartCoordinatePlaneView: public CartesianCoordinatePlaneView
{
private:
	void paintGrid_x( QPainter& painter ) const;
	void paintGrid_y( QPainter& painter ) const;

protected:
	virtual void paintFront( QPainter& painter ) const;
	virtual void paintBack( QPainter& painter ) const;
	virtual void paintGrid( QPainter& painter ) const;

public:
    LinearChartCoordinatePlaneView();
    ~LinearChartCoordinatePlaneView();

public slots:
    virtual void update();
};

}
#endif // THISTLE_LINEARCHARTCOORDINATEPLANEVIEW_H
