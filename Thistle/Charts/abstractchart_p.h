#ifndef ABSTRACTCHART_P_H
#define ABSTRACTCHART_P_H

#include <QRect>
#include <QString>
#include <QFont>
#include <QTime>
#include "../kernel/abstractitemview_p.h"
#include "legends/chartlegend.h"

namespace Thistle {

    class AbstractChart;

    struct AbstractChartPrivate : AbstractItemViewPrivate {

        inline AbstractChart* const q_func() { return q_ptr; }
        AbstractChart* q_ptr;

        QString title;
        QRect titleRect;
        QFont titleFont;

        ChartLegend* legend;

        AbstractChartPrivate( AbstractChart* q );
        ~AbstractChartPrivate();

        void pressEvent();
        void releaseEvent();
    };

}

#endif // ABSTRACTCHART_P_H
