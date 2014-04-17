/*
 This file is part of Marb.

    Marb is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    Marb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with Marb.    If not, see <http://www.gnu.org/licenses/>.

 Marb    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#ifndef TREE_H
#define TREE_H

#include "../kernel/abstractitemview.h"

#include <QPen>
#include <QPointer>

#include "tree_p.h"

class ItemDelegate;

class Tree : public AbstractItemView {
    Q_OBJECT
    Q_DECLARE_PRIVATE( Tree );
protected:
    virtual void updateValues();
    virtual void positionsInTree() = 0;
    virtual void positionsInView() = 0;
    virtual QPointF scan( QModelIndex index, QPointF leftDepth = QPointF( 0, 0 ) ) = 0;
    virtual void setX( QModelIndex index, qreal x );
    virtual void setY( QModelIndex index, qreal y );

    void paintEvent( QPaintEvent* event);
    QModelIndex indexAt(const QPoint &point) const;
    virtual void resizeEvent( QResizeEvent* event );

    virtual void paintItems( QPainter& painter, const QPointF& offset ) const;
    virtual void paintConnections( QPainter& painter, const QPointF& offset ) const;
    virtual void paintConnectionsFor( QPainter& painter, const QModelIndex& index, const QPointF& offset ) const;

    Tree( TreePrivate* d, QWidget* parent = 0 );

public:

    explicit Tree( QWidget* parent = 0 );

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

#endif // TREE_H
