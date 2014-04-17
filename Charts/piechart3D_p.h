#ifndef PIECHART3D_P_H
#define PIECHART3D_P_H

#include <QRect>
#include <QString>

#include "piechart_p.h"
//#include "piechart3D.h"

namespace Marb {

struct PieChart3DPrivate : public PieChartPrivate {
    QList<qreal> angles;
    /*PieChart3D::Render*/ int render;
    QPainterPath front;
    qreal height;
    PieChart3DPrivate();
};

}

#endif // PIECHART3D_P_H
