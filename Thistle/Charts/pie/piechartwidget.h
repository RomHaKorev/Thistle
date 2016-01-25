#ifndef THISTLE_PIECHARTWIDGET_H
#define THISTLE_PIECHARTWIDGET_H

#include <QWidget>

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

class PieChartWidgetPrivate;
class PieChart;

class PieChartWidget : public QWidget
{
	Q_OBJECT
    Q_DECLARE_PRIVATE( PieChartWidget )

protected:
    PieChartWidgetPrivate* d_ptr;
	void resizeEvent( QResizeEvent* ev );
public:
    PieChartWidget( QWidget* parent = 0 );
    ~PieChartWidget();

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;

	void setTitle( const QString& title );
	QString title() const;

	QFont font() const;
	void setFont( const QFont& font );

    PieChart& chart();
};
}

#endif // THISTLE_PIECHARTWIDGET_H
