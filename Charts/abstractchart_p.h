#ifndef ABSTRACTCHART_P_H
#define ABSTRACTCHART_P_H

#include <QRect>
#include <QString>

#include "../kernel/abstractitemview_p.h"

struct AbstractChartPrivate : AbstractItemViewPrivate {
    QRect legendRect;
    QString title;
    QRect titleRect;
    int minBottomMargin;
    AbstractChartPrivate();
};

#endif // ABSTRACTCHART_P_H
