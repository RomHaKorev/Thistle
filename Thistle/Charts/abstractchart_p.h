#ifndef THISTLE_ABSTRACTCHART_P_H
#define THISTLE_ABSTRACTCHART_P_H

#include <QRect>
#include <QString>
#include <QFont>

#include "../kernel/abstractitemview_p.h"
#include "legends/chartlegend.h"
#include "Layers/abstractLayer.h"
#include "serieformat.h"

namespace Thistle
{

class AbstractChart;

struct AbstractChartPrivate : AbstractItemViewPrivate
{

    inline AbstractChart* const q_func()
    {
        return q_ptr;
    }
    AbstractChart* q_ptr;

    QString title;
    QRect titleRect;
    QFont titleFont;

    ChartLegend* legend;

    QMap<int, SerieFormat> style;

    std::list<AbstractLayer*> layers;

    AbstractChartPrivate( AbstractChart* q );
    ~AbstractChartPrivate();

    void pressEvent();
    void releaseEvent();

    void addLayer( AbstractLayer* layer );
    void applyLayer( QPainter& painter );
};

}

#endif // THISTLE_ABSTRACTCHART_P_H
