#ifndef THISTLE_SCATTERCHARTWIDGET_H
#define THISTLE_ScatterChartWidget_H

#include <QWidget>

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

class ScatterChartWidgetPrivate;
class LinearChart;

class ScatterChartWidget : public QWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE( ScatterChartWidget );

protected:
	ScatterChartWidgetPrivate* d_ptr;
	void resizeEvent( QResizeEvent* ev );
public:
	ScatterChartWidget( QWidget* parent = 0 );
	~ScatterChartWidget();

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;

	void setTitle( const QString& title );
	QString title() const;

	QFont font() const;
	void setFont( const QFont& font );

	LinearChart& chart();
};
}

#endif // THISTLE_ScatterChartWidget_H
