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

#ifndef TREE_H
#define TREE_H

#include <QAbstractItemView>
#include <QPen>
#include <QPointer>

class TreeItemDelegate;

class Tree : public QAbstractItemView {
  Q_OBJECT
protected:
  QPen myConnectionPen;

  QSize        myRealSize;
  QRect        myItemRect;
  int          myYDistance;
  int          myXDistance;
  qreal        myDepth;
  qreal        myLeft;
  QPoint       myCentralItemPos;
  QPointF      myItemOffset;
  QPointer<TreeItemDelegate> myDelegate;
  QMap<QModelIndex, QPointF> myItemPos;
  QMap<QModelIndex, QPointF> myItemTreePos;


  virtual void setScrollBarValues() = 0;
  virtual void resizeEvent( QResizeEvent* event );
  QModelIndex moveCursor( QAbstractItemView::CursorAction cursorAction,
                          Qt::KeyboardModifiers modifiers );

  virtual void positionsInTree() = 0;
  virtual void positionsInView() = 0;
  virtual QPointF scan( QModelIndex index, QPointF leftDepth = QPointF( 0, 0 ) ) = 0;
  virtual void setX( QModelIndex index, qreal x );
  virtual void setY( QModelIndex index, qreal y );

  void paintEvent( QPaintEvent* event);
  virtual void paintItems( QPainter& painter, QPointF offset );
  virtual void paintConnections( QPainter& painter, QPointF offset );
  virtual void paintConnectionsFor( QPainter& painter, QModelIndex index, QPointF offset );

public:

  explicit Tree(QWidget* parent = 0);

  int         horizontalOffset() const;
  int         verticalOffset() const;
  QModelIndex indexAt( const QPoint& point ) const;
  bool        isIndexHidden( const QModelIndex& index ) const;
  void        setSelection( const QRect&, QItemSelectionModel::SelectionFlags command );
  void        scrollTo( const QModelIndex& index, ScrollHint hint = EnsureVisible );
  QRect       visualRect( const QModelIndex& index ) const;
  QRegion     visualRegionForSelection( const QItemSelection& selection ) const;


  virtual QRect itemRect( const QModelIndex& index ) const;
  int rows( const QModelIndex& index ) const;

  /*  DESIGN METHODS */
  void setItemSize( const QSize& s );
  void setConnectionPen( const QPen& pen );
  QPen connectionPen() const;
  void setItemSpacing( int w, int h );


protected slots:
  void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
  void rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end);
  void rowsInserted(const QModelIndex &parent, int start, int end);
  
public slots:
  void show();
  virtual bool save( QString filename ) = 0;
};

#endif // TREE_H
