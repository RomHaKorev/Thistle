#ifndef THISTLE_KIVIATCHARTWIDGET_P_H
#define THISTLE_KIVIATCHARTWIDGET_P_H

#include <QtCore/QRect>
#include <QLabel>

#include "../kiviatchart.h"
#include "../../line/linelegendview.h"


namespace Thistle
{
class KiviatChartWidget;

struct KiviatChartWidgetPrivate
{
    QPointer<KiviatChartWidget> parent;
	KiviatChart chart;
	LineLegendView legendView;
    QLabel title;

	KiviatChartWidgetPrivate( KiviatChartWidget* parent );
	virtual ~KiviatChartWidgetPrivate();

    void resize( const QRect& rect );

    void setModel( QAbstractItemModel* model );
    QAbstractItemModel* model() const;
};

}

#endif // THISTLE_KIVIATCHARTWIDGET_P_H
