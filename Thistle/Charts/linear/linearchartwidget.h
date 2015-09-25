#ifndef THISTLE_LINEARCHARTWIDGET_H
#define THISTLE_LINEARCHARTWIDGET_H

#include <QWidget>

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

class LinearChartWidgetPrivate;
class LinearChart;

class LinearChartWidget : public QWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE( LinearChartWidget );

protected:
	LinearChartWidgetPrivate* d_ptr;
	void resizeEvent( QResizeEvent* ev );
public:
	LinearChartWidget( QWidget* parent = 0 );
	~LinearChartWidget();

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;

	void setTitle( const QString& title );
	QString title() const;

	QFont font() const;
	void setFont( const QFont& font );

	LinearChart& chart();
};
}

#endif // THISTLE_LINEARCHARTWIDGET_H
