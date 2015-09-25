#ifndef THISTLE_KIVIATCHARTWIDGET_P_H
#define THISTLE_KIVIATCHARTWIDGET_P_H

#include <QtCore/QRect>
#include <QLabel>

#include "../kiviatchart.h"
#include "../../linear/linearlegendview.h"


namespace Thistle
{

class LinearLegendView;
class KiviatChartWidget;

struct KiviatChartWidgetPrivate
{
    QPointer<KiviatChartWidget> parent;
	KiviatChart chart;
    LinearLegendView legendView;
    QLabel title;

	KiviatChartWidgetPrivate( KiviatChartWidget* parent );
	virtual ~KiviatChartWidgetPrivate();

    void resize( const QRect& rect );

    void setModel( QAbstractItemModel* model );
    QAbstractItemModel* model() const;
};

}

#endif // THISTLE_KIVIATCHARTWIDGET_P_H
