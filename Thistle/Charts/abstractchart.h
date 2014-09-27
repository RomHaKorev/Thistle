#ifndef ABSTRACTCHART_H
#define ABSTRACTCHART_H

#include "../kernel/abstractitemview.h"

#include <QWidget>
#include <QPaintEvent>

namespace Thistle
{

class ChartLegend;
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
    virtual QPainterPath itemPath(const QModelIndex& index) const;
    virtual void updateRects() = 0;

    virtual bool event( QEvent* ev );
    virtual void resizeEvent( QResizeEvent* ev );
    virtual void paintEvent(QPaintEvent* event);
    virtual void paintChart( QPainter& painter ) = 0;

    AbstractChart( AbstractChartPrivate* d, QWidget* parent = 0 );

    QModelIndex nearestItem( const QPointF& pos ) const;

public:
    AbstractChart( QWidget* parent = 0 );
    ~AbstractChart();

    virtual QModelIndex indexAt(const QPoint& point) const;

    void setTitle( const QString& title );

    const QFont& titleFont() const;
    void setTitleFont( const QFont& font );

    void setLegend( ChartLegend* legend );
    ChartLegend* legend() const;

public slots:
    void updateValues();
};

}

#endif //ABSTRACTCHART_H
