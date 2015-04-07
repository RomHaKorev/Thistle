#ifndef THISTLE_LINEARCHARTWIDGET_P_H
#define THISTLE_LINEARCHARTWIDGET_P_H

#include <QRect>

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

class LinearLegendView;
class LinearChart;
class LinearChartWidget;

struct LinearChartWidgetPrivate
{
    LinearChartWidget* parent;
    LinearChart* chart;
    LinearLegendView* legendView;
    QLabel* title;

    LinearChartWidgetPrivate( LinearChartWidget* parent );
    ~LinearChartWidgetPrivate(); 

    void resize( const QRect& rect );

    void setModel( QAbstractItemModel* model );
    QAbstractItemModel* model() const;
};

}

#endif // THISTLE_LINEARCHARTWIDGET_P_H
