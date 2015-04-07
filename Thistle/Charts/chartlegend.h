#ifndef THISTLE_CHARTLEGEND_H
#define THISTLE_CHARTLEGEND_H

#include <QRect>
#include <QPainter>

namespace Thistle
{
class     AbstractItemView;

class ChartLegend
{
private:
    AbstractItemView* chart;

public:
    QRect area;
    ChartLegend( AbstractItemView* chart );
    ~ChartLegend();

    virtual void paint( QPainter& painter ) const;

protected:
    virtual void paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const;
};

}

#endif // THISTLE_CHARTLEGEND_H
