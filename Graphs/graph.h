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

#ifndef GRAPH_H
#define GRAPH_H

#include <QTimer>
#include <QTime>
#include "../kernel/marbabstractitemview.h"
#include "node.h"

class GraphModel;

class Graph : public MarbAbstractItemView {
    Q_OBJECT
protected:
    QMap<QModelIndex, Node> myItemPos;
    QTimer myTimer;
    qreal oldLength;
    QSizeF myRealSize;
    QPointF myItemOffset;

    bool myMovableItem;
    bool myElasticItem;
    QModelIndex myMovedItem;
    QTime myDragDropTime;

    qreal myWeight;

    GraphModel* myModel;

    virtual QModelIndex indexAt(const QPoint &point) const;
    virtual void setScrollBarValues();

public:
    explicit Graph(QWidget *parent = 0);

        void updateValues();

    virtual QPainterPath itemPath(const QModelIndex &index) const;

    void calculateForces( QModelIndex& index );

    void mousePressEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );

    void paintEvent( QPaintEvent* event );
    void paintEdges( QPainter& painter, QPointF offset = QPointF( 0, 0 ) );
    void paintItems( QPainter& painter, QPointF offset = QPointF( 0, 0 ) );
    void paintEdge( QPainter& painter, QModelIndex idx1, QModelIndex idx2, Edge::Type type );
    void paintArrow( QPainter& painter, QLineF line );

    void setNodeWeight( qreal weight );
    void setElasticNode( bool enabled );

    void setModel(QAbstractItemModel *model);

public slots:
    virtual bool save( QString filename );

protected slots:
    void processTimer();
};

#endif // GRAPH_H
