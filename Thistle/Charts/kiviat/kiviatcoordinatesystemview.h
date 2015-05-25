#ifndef THISTLE_KIVIATCOORDINATESYSTEMVIEW_H
#define THISTLE_KIVIATCOORDINATESYSTEMVIEW_H

#include "../base/abstractcoordinatesystemview.h"

namespace Thistle
{

class KiviatCoordinateSystemView : public AbstractCoordinateSystemView
{

protected:
	virtual void paintBack( QPainter& painter ) const;
	virtual void paintFront( QPainter& painter ) const;
	virtual void update();
public:
	KiviatCoordinateSystemView();
	~KiviatCoordinateSystemView();

private:
	
};

} // namespace Thistle

#endif // THISTLE_KIVIATCOORDINATESYSTEMVIEW_H
