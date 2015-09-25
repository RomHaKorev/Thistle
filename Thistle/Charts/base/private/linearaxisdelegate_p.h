#ifndef THISTLE_LinearAxisDelegate_P_H
#define THISTLE_LinearAxisDelegate_H

#include <QFont>
#include <QPen>
namespace Thistle
{
class AbstractCoordinateSystemView;

struct LinearAxisDelegatePrivate
{
	LinearAxisDelegatePrivate( AbstractCoordinateSystemView* parent );
	~LinearAxisDelegatePrivate();

	AbstractCoordinateSystemView* parent;

	QFont font;
	QPen base;
	QPen tick;
	QPen textPen;
	qreal tickReduceFactor;
	int tickStyle;
};

}

#endif // THISTLE_LinearAxisDelegate_H
