#ifndef THISTLE_COORDINATESYSTEMVIEWPRIVATE_P_H
#define THISTLE_COORDINATESYSTEMVIEWPRIVATE_P_H

#include <QtGui/QPen>
#include <QtGui/QFont>
#include <QtCore/QMap>

#include "../linearaxisdelegate.h"

class QAbstractItemModel;

namespace Thistle
{
class AbstractCoordinateSystem;
class AbstractCoordinateSystemView;

struct AbstractCoordinateSystemViewPrivate
{
	AbstractCoordinateSystemViewPrivate( AbstractCoordinateSystem* coordSystem, AbstractCoordinateSystemView* coordSysView );
	virtual ~AbstractCoordinateSystemViewPrivate();
	virtual AbstractCoordinateSystem* coordinateSystem() const;

	QRect rect;
	QFont font;
	QPen axisPen;
	QPen tickPen;
	QPen textPen;
	QAbstractItemModel* model;
	QSharedPointer<LinearAxisDelegate> defaultDelegate;
	QMap< int, QSharedPointer<LinearAxisDelegate> > delegates;

	void setDelegateFor( int index, QSharedPointer<LinearAxisDelegate> delegate );
	QSharedPointer<LinearAxisDelegate> delegateFor( int index );

protected:
	AbstractCoordinateSystem* coordSystem;
};

}

#endif // THISTLE_COORDINATESYSTEMVIEWPRIVATE_P_H
