#ifndef TREELAYOUT_P_H
#define TREELAYOUT_P_H

#include <QRect>
#include <QMap>
#include <QPointF>
#include <QModelIndex>
#include <QMargins>

#include <QPolygonF>

namespace Thistle
{

class TreeView;

struct TreeLayoutPrivate
{
    TreeLayoutPrivate();
    Qt::Orientation orientation;

    TreeView* view;

    QRect rect;
    QSize size;
    QRect boundingRect;
    QMargins margins;
    QMargins spacing;
    qreal depth;
    qreal left;
    QPoint centralItemPos;
    QPointF itemOffset;
    QMap<QModelIndex, QPointF> itemPos;
    QMap<QModelIndex, QPointF> itemTreePos;

    void calculateTreePositions();
    void calculateViewPositions();
    virtual QPointF scan( const QModelIndex& index, const QPointF& leftDepth = QPointF( 0, 0 ) );
    void setX( const QModelIndex& index, qreal x );
    void setY( const QModelIndex& index, qreal y );

    virtual QRect itemRect() const;
    virtual QRect itemRect( const QModelIndex& index ) const;

    virtual void calculateItemRect( const QSize& size );
    virtual void calculateItemRect( const QMargins& margins );
};

}

#endif // TreeLayoutPrivate
