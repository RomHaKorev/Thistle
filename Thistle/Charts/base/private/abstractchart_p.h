#ifndef THISTLE_ABSTRACTCHART_P_H
#define THISTLE_ABSTRACTCHART_P_H

#include <QRect>
#include <QString>
#include <QFont>

#include "../../../kernel/abstractitemview_p.h"
//#include "Layers/abstractLayer.h"
#include "../../base/serieformat.h"

namespace Thistle
{

class AbstractChart;

struct AbstractChartPrivate : AbstractItemViewPrivate
{

    inline AbstractChart* q_func()
    {
        return q_ptr;
    }
    AbstractChart* q_ptr;

    QMap<int, SerieFormat> style;

    AbstractChartPrivate( AbstractChart* q );
    ~AbstractChartPrivate();

    void pressEvent();
    void releaseEvent();

#if 0
    std::list<AbstractLayer*> layers;
    void addLayer( AbstractLayer* layer );
    void applyLayer( QPainter& painter );
#endif
};

}

#endif // THISTLE_ABSTRACTCHART_P_H
