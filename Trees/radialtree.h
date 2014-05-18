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

#ifndef RADIALTREE_H
#define RADIALTREE_H

#include "abstracttree.h"
#include "radialtree_p.h"

namespace Thistle {

/*!
\class RadialTree
\brief The RadialTree implements a tree representation of item from the given model.

Contrary to QTreeView, a radial tree is not a hierarchical list: the root is placed at the center and all children are placed on concentric circles.

\sa Thistle::VerticalTree, Thistle::HorizontaTree, Thistle::AbstractTree
*/
class RadialTree : public AbstractTree {
    Q_OBJECT
    Q_DECLARE_PRIVATE( RadialTree );
protected:
    void updatePerimeter();

    virtual void setScrollBarValues();
    virtual void paintCircles( QPainter& painter, const QPointF& offset ) const;
    //virtual void paintItems( QPainter& painter, QPointF offset );
    virtual void paintConnections( QPainter& painter, QPointF offset ) const;
    //virtual void paintConnectionsFor( QPainter& painter, QModelIndex index, QPointF offset );
    RadialTree( RadialTreePrivate* d, QWidget* parent = 0 );
public:
    explicit RadialTree( QWidget* parent = 0 );
    virtual void positionsInTree();
    virtual void positionsInView();
    virtual QPointF scan(QModelIndex index, QPointF leftDepth);
    void setRotateText( bool rotate = true );
};

}
#endif // RADIALTREE_H
