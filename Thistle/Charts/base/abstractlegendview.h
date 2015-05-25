#ifndef THISTLE_ABSTRACTLEGENDVIEW_H
#define THISTLE_ABSTRACTLEGENDVIEW_H


#include <QPainter>
#include <QRect>

#include "../../kernel/abstractitemview.h"

namespace Thistle
{

class AbstractChart;
class AbstractLegendViewPrivate;

class AbstractLegendView : public AbstractItemView
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( AbstractLegendView );
private:
    virtual void setScrollBarValues();

    virtual QModelIndex indexAt(const QPoint &point) const;
    virtual void setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command );
    virtual QPainterPath itemPath( const QModelIndex& index ) const;
    virtual void updateValues();

    virtual void paintSerie( QPainter &painter, int serie, const QRect& rect ) const = 0;
    virtual void paintEvent( QPaintEvent* ev );

    bool isActiveColumn( int column ) const;

protected:
    AbstractLegendView( AbstractLegendViewPrivate* d, QWidget* parent = 0 );

public:
    AbstractLegendView( AbstractChart* chart, QWidget* parent = 0 );
    virtual ~AbstractLegendView();

    void resizeEvent( QResizeEvent* ev );

    void updateSizeHint( const QSize& source );

    QSize sizeHint() const;

    void parentManageSize( bool leave );
};

}


#endif // THISTLE_ABSTRACTLEGENDVIEW_H