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

#ifndef RADIALTREE_H
#define RADIALTREE_H

#include "tree.h"

class RadialTree : public Tree {
  Q_OBJECT

protected:
  qreal myDiagonal;
  qreal myPerimeter;
  qreal myRadius;
  QList<QModelIndex> myOrderedIndexes;
  qreal my_rotateText;
  qreal my_maxRad;
  QPointF my_currentCenter;

  void paintEvent( QPaintEvent* ev );
  void updatePerimeter();

  virtual void setScrollBarValues();
public:
  explicit RadialTree( QWidget* parent = 0 );
  virtual void positionsInTree();
  virtual void positionsInView();
  virtual QPointF scan(QModelIndex index, QPointF leftDepth);
  void setRotateText( bool rotate = true );

signals:
  
public slots:
};

#endif // RADIALTREE_H
