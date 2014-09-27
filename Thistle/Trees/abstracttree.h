/*
 This file is part of Thistle.

    Thistle is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    Thistle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with Thistle.    If not, see <http://www.gnu.org/licenses/>.

 Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#ifndef TREE_H
#define TREE_H

#include "../kernel/abstractitemview.h"

#include <QPen>
#include <QPointer>

#include "abstracttree_p.h"

namespace Thistle
{

class ItemDelegate;

class AbstractTree : public AbstractItemView
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( AbstractTree );
protected:
    virtual void updateValues();

    virtual void positionsInTree() = 0;

    virtual void positionsInView() = 0;
    virtual QPointF scan( const QModelIndex& index, const QPointF& leftDepth = QPointF( 0, 0 ) ) = 0;
    virtual void setX( const QModelIndex& index, qreal x );
    virtual void setY( const QModelIndex& index, qreal y );

    void paintEvent( QPaintEvent* event);
    QModelIndex indexAt(const QPoint &point) const;
    virtual void resizeEvent( QResizeEvent* event );

    virtual void paintItems( QPainter& painter, const QPointF& offset ) const;
    virtual void paintConnections( QPainter& painter, const QPointF& offset ) const;
    virtual void paintConnectionsFor( QPainter& painter, const QModelIndex& index, const QPointF& offset ) const;

    AbstractTree( AbstractTreePrivate* d, QWidget* parent = 0 );
    ~AbstractTree();
public:

    explicit AbstractTree( QWidget* parent = 0 );

    virtual QRectF itemRect( const QModelIndex& index ) const;
    virtual QPainterPath itemPath( const QModelIndex& index ) const;

    /*    DESIGN METHODS */
    void setItemSize( const QSize& s );
    void setConnectionPen( const QPen& pen );
    QPen connectionPen() const;
    void setItemSpacing( int w, int h );

public slots:
    void show();
};

}
#endif // TREE_H
