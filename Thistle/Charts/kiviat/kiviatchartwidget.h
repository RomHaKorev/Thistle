#ifndef THISTLE_KIVIATCHARTWIDGET_H
#define THISTLE_KIVIATCHARTWIDGET_H

#include <QFrame>

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

class KiviatChartWidgetPrivate;
class KiviatChart;

class KiviatChartWidget: public QFrame
{
	Q_OBJECT
		Q_DECLARE_PRIVATE( KiviatChartWidget );

protected:
	KiviatChartWidgetPrivate* d_ptr;
	void resizeEvent( QResizeEvent* ev );
public:
	KiviatChartWidget( QWidget* parent = 0 );
	virtual ~KiviatChartWidget();

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;

	void setTitle( const QString& title );
	QString title() const;

	QFont font() const;
	void setFont( const QFont& font );

	KiviatChart& chart();
};
}

#endif // THISTLE_KIVIATCHARTWIDGET_H
