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

#ifndef RADIALTREE_H
#define RADIALTREE_H

#include "tree.h"
#include "radialtree_p.h"

class RadialTree : public Tree {
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

signals:

public slots:
    virtual bool save( const QString& filename ) const;
};
#endif // RADIALTREE_H
