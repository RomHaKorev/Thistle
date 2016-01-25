#ifndef THISTLE_ABSTRACTCHART_H
#define THISTLE_ABSTRACTCHART_H

#include "../../Core/abstractitemview.h"
#include "serieformat.h"
#include "serieformatproxy.h"

#include <QWidget>
#include <QPaintEvent>

namespace Thistle
{

class ChartLegend;
class AbstractLayer;
class AbstractChartPrivate;

class AbstractChart :  public AbstractItemView
{
	Q_DECLARE_PRIVATE( AbstractChart );

protected:
	virtual void process();

	virtual void scan() = 0;

	virtual void setScrollBarValues();
	virtual void updateRects() = 0;

	virtual void resizeEvent( QResizeEvent* ev );
	virtual void paintEvent(QPaintEvent* event) = 0;

	AbstractChart( AbstractChartPrivate* d, QWidget* parent = 0 );

	virtual QModelIndex nearestItem( const QPointF& pos ) const;

public:
	AbstractChart( QWidget* parent = 0 );
	virtual ~AbstractChart();

	virtual QModelIndex indexAt(const QPoint& point) const;

#if 0
	void addLayer( AbstractLayer* layer );
#endif

public slots:
	virtual void updateValues();
};

}

#endif //ABSTRACTCHART_H
