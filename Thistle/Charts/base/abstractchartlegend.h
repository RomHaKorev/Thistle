#ifndef THISTLE_ABSTRACTCHARTLEGEND_H
#define THISTLE_ABSTRACTCHARTLEGEND_H

#include <QRect>
#include <QPainter>

namespace Thistle
{
class AbstractItemView;

class AbstractChartLegend
{
private:
    AbstractItemView* chart;
	QRect area;

public:
	AbstractChartLegend( AbstractItemView* chart );
	virtual ~AbstractChartLegend();

    virtual void paint( QPainter& painter ) const;

protected:
    virtual void paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const;
};

}

#endif // THISTLE_ABSTRACTCHARTLEGEND_H
