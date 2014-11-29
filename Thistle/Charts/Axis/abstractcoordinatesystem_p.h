#ifndef THISTLE_ABSTRACTCOORDINATESYSTEM_P_H
#define THISTLE_ABSTRACTCOORDINATESYSTEM_P_H

#include <QRect>
#include <QFont>
#include <QAbstractItemModel>
#include <QPen>

#include "linearaxis.h"

namespace Thistle
{

class AbstractCoordinateSystemPrivate
{
protected:
    QList<LinearAxis> axis;
public:
    QRect valuesRect;
    QRect chartRect;
    QFont font;
    QPen axisPen;
    QPen tickPen;
    QPen textPen;
    QAbstractItemModel* model;

    AbstractCoordinateSystemPrivate();

};

}

#endif // THISTLE_ABSTRACTCOORDINATESYSTEM_P_H
