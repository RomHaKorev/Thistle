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

#ifndef VERTICALTREE_H
#define VERTICALTREE_H

#include "abstracttree.h"

#include "verticaltree_p.h"


class QStandardItemModel;

namespace Thistle
{


class VerticalTree : public AbstractTree
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( VerticalTree );
public:
    enum ConnectorType
    {
        Straight = 0,
        Elbow        = 1
    };
    Q_ENUMS( ConnectorType );

protected:
    virtual void setScrollBarValues();

public:
    explicit VerticalTree( QWidget* parent = 0 );
    ~VerticalTree();

    virtual void positionsInView();
    virtual void positionsInTree();
    virtual QPointF scan( const QModelIndex& index, const QPointF& leftDepth = QPointF( 0, 0 ) );

    void paintConnectionsElbow( QPainter& painter, const QModelIndex& id ) const;
    void paintConnectionsStraight( QPainter& painter, const QModelIndex& id ) const;
    virtual void paintConnectionsFor( QPainter& painter, const QModelIndex& index, const QPointF& offset ) const;

    void setConnectorType( ConnectorType type );
    ConnectorType connectorType() const;
};

}

#endif // VERTICALTREE_H
