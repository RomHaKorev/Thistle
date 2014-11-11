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

#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "../kernel/abstractitemview.h"

#include <QPen>
#include <QPointer>

#include "treeview_p.h"
#include "treelayout.h"

namespace Thistle
{

class ItemDelegate;

class TreeView : public AbstractItemView
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( TreeView );
protected:
    virtual void updateValues();

    void setScrollBarValues();

    void paintEvent( QPaintEvent* event);
    QModelIndex indexAt(const QPoint &point) const;
    virtual void resizeEvent( QResizeEvent* event );

    virtual void paintItems( QPainter& painter, const QPointF& offset ) const;
    virtual void paintConnections( QPainter& painter, const QPointF& offset ) const;
    virtual void paintConnectionsFor( QPainter& painter, const QModelIndex& index, const QPointF& offset ) const;

    TreeView( TreeViewPrivate* d, QWidget* parent = 0 );
    ~TreeView();
public:

    explicit TreeView( QWidget* parent = 0 );

    virtual QRectF itemRect( const QModelIndex& index ) const;
    virtual QPainterPath itemPath( const QModelIndex& index ) const;

    /*    DESIGN METHODS */
    void setConnectionPen( const QPen& pen );
    QPen connectionPen() const;

    void setLayout( TreeLayout* layout );
    TreeLayout* layout() const;
};


}

#endif //TREEVIEW_H