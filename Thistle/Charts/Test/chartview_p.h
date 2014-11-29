#ifndef THISTLE_ABSTRACTCHART_P_H
#define THISTLE_ABSTRACTCHART_P_H

#include <QRect>
#include <QString>
#include <QFont>

#include "../kernel/abstractitemview_p.h"
#include "serieformat.h"

namespace Thistle
{

class ChartView;
class AxisView;
class ChartLegend;

struct ChartViewPrivate : AbstractItemViewPrivate
{

    inline ChartView* const q_func()
    {
        return q_ptr;
    }
    ChartView* q_ptr;

    QString title;
    QRect titleRect;
    QFont titleFont;

    AxisView* axisView;
    ChartLegend* legend;

    QMap<int, SerieFormat> style;

    ChartViewPrivate( ChartView* q );
    ~ChartViewPrivate();
};

}

#endif // THISTLE_ABSTRACTCHART_P_H
