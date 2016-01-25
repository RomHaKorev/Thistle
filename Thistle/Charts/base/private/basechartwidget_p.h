#ifndef THISTLE_BASECHARTWIDGET_P_H
#define THISTLE_BASECHARTWIDGET_P_H

#include <QtCore/QRect>
#include <QLabel>
#include <QPointer>
#include <QAbstractItemModel>
#include <QWidget>

namespace Thistle
{
class AbstractChart;
class AbstractLegendView;

struct BaseChartWidgetPrivate
{
	QPointer<AbstractChart> chart;
	QPointer<AbstractLegendView> legendView;
	QPointer<QLabel> title;

	BaseChartWidgetPrivate( QWidget* parent );
	BaseChartWidgetPrivate( QWidget* parent, QPointer<AbstractChart> chart, QPointer<AbstractLegendView> legend );
	virtual ~BaseChartWidgetPrivate();

	void resize( const QRect& rect );

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;
};

}

#endif // THISTLE_BASECHARTWIDGET_P_H
