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
  along with Marb.  If not, see <http://www.gnu.org/licenses/>.

 Marb  Copyright (C) 2013  Dimitry Ernot
*/

#ifndef GRAPH_H
#define GRAPH_H

#include <QTimer>
#include <QTime>
#include "../marbabstractitemview.h"
#include "node.h"


class Graph : public MarbAbstractItemView {
  Q_OBJECT
protected:
  QList<Edge> myEdges;
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

  virtual void setScrollBarValues();
  void resizeEvent( QResizeEvent* event );

public:
  explicit Graph(QWidget *parent = 0);

    void updateValues();

  virtual QPainterPath itemPath(const QModelIndex &index) const;

  void calculateForces( QModelIndex& index );
  void addEdge( QModelIndex idx1, QModelIndex idx2, Edge::Type type = Edge::NoArrow );
  void addEdge( int row1, int col1, int row2, int col2, Edge::Type type = Edge::NoArrow );

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

public slots:
  virtual bool save( QString filename );

protected slots:
  void processTimer();
};

#endif // GRAPH_H
