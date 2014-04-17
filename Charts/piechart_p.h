#ifndef PIECHART_P_H
#define PIECHART_P_H

#include <QRect>
#include <QString>

#include "abstractchart_p.h"

namespace Marb {

struct PieChartPrivate : AbstractChartPrivate {
    QRect rect;
    qreal total;
    bool splitted;
    bool ring;
    qreal startAngle;
    PieChartPrivate();
};

}

#endif // PIECHART_P_H
