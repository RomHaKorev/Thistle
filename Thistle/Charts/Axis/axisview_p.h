#ifndef THISTLE_AXISVIEW_P_H
#define THISTLE_AXISVIEW_P_H

#include <QPen>
#include <QFont>

class QAbstractItemModel;

namespace Thistle
{
class CartesianCoordinateSystem;

struct AxisViewPrivate
{
    AxisViewPrivate();

    CartesianCoordinateSystem* coordinateSystem;

    QRect rect;
    QFont font;
    QPen axisPen;
    QPen tickPen;
    QPen textPen;
    QAbstractItemModel* model;
};

}

#endif // THISTLE_AXISVIEW_P_H
