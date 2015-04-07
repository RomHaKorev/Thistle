#ifndef THISTLE_ABSTRACTCOORDINATESYSTEM_P_H
#define THISTLE_ABSTRACTCOORDINATESYSTEM_P_H

#include <QRect>
#include <QFont>
#include <QAbstractItemModel>
#include <QPen>

#include "../abstractaxis.h"

namespace Thistle
{

class AbstractAxis;

class AbstractCoordinateSystemPrivate
{
public:
    QRectF rect;
    QFont font;
    QPen axisPen;
    QPen tickPen;
    QPen textPen;
    QAbstractItemModel* model;
    QPointF valueOffset;

    AbstractCoordinateSystemPrivate();
    virtual ~AbstractCoordinateSystemPrivate();

	virtual AbstractAxis& axis( unsigned int index ) = 0;
	virtual unsigned int axisCount() const = 0;

};

}

#endif // THISTLE_ABSTRACTCOORDINATESYSTEM_P_H
