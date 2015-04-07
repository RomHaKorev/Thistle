#ifndef THISTLE_CHARTVIEW_H
#define THISTLE_CHARTVIEW_H

#include "../../kernel/abstractitemview.h"
#include "../serieformat.h"

#include <QWidget>
#include <QPaintEvent>

namespace Thistle
{

class ChartLegend;
class ChartViewPrivate;

class ChartView :  public AbstractItemView
{
    Q_DECLARE_PRIVATE( ChartView );

protected:
    virtual void process();

    void calculateLegendRect( const QRect& source );

    virtual void setScrollBarValues();
    virtual QPainterPath itemPath(const QModelIndex& index) const;
    virtual void doLayout();

    virtual void resizeEvent( QResizeEvent* ev );
    virtual void paintEvent(QPaintEvent* event);

    ChartView( ChartViewPrivate* d, QWidget* parent = 0 );

public:
    ChartView( QWidget* parent = 0 );
    ~ChartView();

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

#endif //THISTLE_CHARTVIEW_H
