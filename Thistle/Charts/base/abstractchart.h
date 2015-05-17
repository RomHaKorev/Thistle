#ifndef THISTLE_ABSTRACTCHART_H
#define THISTLE_ABSTRACTCHART_H

#include "../../kernel/abstractitemview.h"
#include "serieformat.h"

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

    virtual void defineRects() = 0;
    virtual void scan() = 0;
    void calculateLegendRect( const QRect& source );

    virtual void setScrollBarValues();
    virtual QPainterPath itemPath(const QModelIndex& index) const = 0;
    virtual void updateRects() = 0;

    //virtual bool event( QEvent* ev );
    virtual void resizeEvent( QResizeEvent* ev );
    virtual void paintEvent(QPaintEvent* event) = 0;

    AbstractChart( AbstractChartPrivate* d, QWidget* parent = 0 );

    QModelIndex nearestItem( const QPointF& pos ) const;

public:
    AbstractChart( QWidget* parent = 0 );
    ~AbstractChart();

    virtual QModelIndex indexAt(const QPoint& point) const;

#if 0
    void addLayer( AbstractLayer* layer );
#endif
    SerieFormat serieFormat( int column ) const;
    void setSerieFormat( int column, SerieFormat style);

public slots:
    void updateValues();
};

}

#endif //ABSTRACTCHART_H
