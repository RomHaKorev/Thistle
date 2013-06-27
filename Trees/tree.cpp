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

#include "tree.h"

#include "treeitemdelegate.h"

#include <QScrollBar>
#include <QDebug>

#include <QStandardItem>

#include "../Marb.h"

Tree::Tree(QWidget *parent) : QAbstractItemView( parent ) {
  myRealSize = QSize( 200, 200);
  myItemRect = QRect( -40, -20, 80, 40 );
  myYDistance = 50;
  myXDistance = 20;
  myDepth = 0;
  myLeft = 0;
  myConnectionPen = QPen( QColor(Marb::LightGray), 2 );

  myDelegate = new TreeItemDelegate( this );
  setItemDelegate( myDelegate );
}

int Tree::horizontalOffset() const {
  return horizontalScrollBar()->value();
}

void Tree::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command ) {
  QRect selectRect = rect;

  int rows = model()->rowCount(rootIndex());
  QModelIndexList indexes;

  for (int row = 0; row < rows; ++row) {
    QModelIndex index = model()->index(row, 0, rootIndex());
    if ( !itemRect( index ).intersect( selectRect ).isEmpty() ) {
      indexes.append( index );
    }
  }

  if ( indexes.isEmpty() ) {
    QModelIndex dummyIndex;
    QItemSelection selection(dummyIndex, dummyIndex);
    selectionModel()->select(selection, command);
    return;
  }
    int firstRow = indexes[0].row();
    int lastRow = indexes[0].row();

    for (int i = 1; i < indexes.size(); ++i) {
        firstRow = qMin(firstRow, indexes[i].row());
        lastRow = qMax(lastRow, indexes[i].row());
    }

    QItemSelection selection(
                              model()->index(firstRow, 0, rootIndex()),
                              model()->index(lastRow, 0, rootIndex())
                            );

    selectionModel()->select(selection, command);
    viewport()->update();
}

QModelIndex Tree::indexAt(const QPoint &point) const {
  QPoint p = point - myCentralItemPos - QPoint( horizontalScrollBar()->value(), verticalScrollBar()->value() );
  foreach( QModelIndex id, myItemPos.keys() ) {
    QRect r = itemRect( id );
    if ( r.contains( p ) ) {
      return id;
    }
  }
  return QModelIndex();
}

bool Tree::isIndexHidden(const QModelIndex& /*index*/ ) const {
  return false;
}

QModelIndex Tree::moveCursor( QAbstractItemView::CursorAction cursorAction,
                       Qt::KeyboardModifiers /*modifiers*/ ) {
  switch( cursorAction ) {
  case QAbstractItemView::MoveDown:
  break;
  default:
  break;
  }

  return QModelIndex();
}

int Tree::rows(const QModelIndex &index) const {
  return model()->rowCount(model()->parent(index));
}

void Tree::scrollTo(const QModelIndex& index, ScrollHint hint ) {
  Q_UNUSED( hint )
  QRect area = viewport()->rect();
  QRect rect = visualRect(index);

  horizontalScrollBar()->setValue( area.width() / 2 );
  return;

  if (rect.left() < area.left())
      horizontalScrollBar()->setValue(
          horizontalScrollBar()->value() + rect.left() - area.left());
  else if (rect.right() > area.right())
      horizontalScrollBar()->setValue(
          horizontalScrollBar()->value() + qMin(
              rect.right() - area.right(), rect.left() - area.left()));

  if (rect.top() < area.top())
      verticalScrollBar()->setValue(
          verticalScrollBar()->value() + rect.top() - area.top());
  else if (rect.bottom() > area.bottom())
      verticalScrollBar()->setValue(
          verticalScrollBar()->value() + qMin(
              rect.bottom() - area.bottom(), rect.top() - area.top()));

  update();
}

int Tree::verticalOffset() const {
  return verticalScrollBar()->value();
}

QRect Tree::visualRect(const QModelIndex &index) const {
  QRect rect = itemRect(index).translated( myCentralItemPos );
  return rect;
}

QRegion Tree::visualRegionForSelection(const QItemSelection &selection) const {
  int ranges = selection.count();

  QRegion region;
  for (int i = 0; i < ranges; ++i) {

      QItemSelectionRange range = selection.at(i);
      for (int row = range.top(); row <= range.bottom(); ++row) {
        QModelIndex index = model()->index(row, 0, rootIndex());
        region += visualRect(index);
      }
  }
  viewport()->update();
  return region;
}


void Tree::setItemSpacing(int w, int h) {
  myXDistance = w;
  myYDistance = h;
  positionsInView();
  update();
}


void Tree::setItemSize( const QSize& s ) {
  myItemRect.setRect( -s.width() / 2, -s.height() / 2, s.width(), s.height() );
  positionsInView();
}

void Tree::setConnectionPen( const QPen& pen ) {
  myConnectionPen = pen;
}

QPen Tree::connectionPen() const {
  return myConnectionPen;
}

QRect Tree::itemRect(const QModelIndex &index) const {
  QPointF p = myItemPos.value( index ) - QPointF( horizontalOffset(), verticalOffset() );
  return myItemRect.translated( p.toPoint() );
}

void Tree::setX( QModelIndex index, qreal x ) {
  if ( !myItemTreePos.contains( index ) ) {
    myItemTreePos.insert( index, QPointF() );
  }

  myItemTreePos[ index ].setX( x );
}

void Tree::setY( QModelIndex index, qreal y ) {
  if ( !myItemTreePos.contains( index ) ) {
    myItemTreePos.insert( index, QPointF() );
  }

  myItemTreePos[ index ].setY( y );
}

/***************************************
**
**        PROTECTED SLOTS
**
***************************************/

void Tree::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  QAbstractItemView::dataChanged(topLeft, bottomRight);
  update( model()->index(0, 0) );
}

void Tree::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end) {
  QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
  positionsInTree();
  viewport()->update();
  setScrollBarValues();
}

void Tree::rowsInserted(const QModelIndex& /*parent*/, int /*start*/, int /*end*/) {
  positionsInTree();
  viewport()->update();
  setScrollBarValues();
}

void Tree::resizeEvent( QResizeEvent* event ) {
  QAbstractItemView::resizeEvent( event );
  //setScrollBarValues();
  positionsInView();
}

/***************************************
**
**        PUBLIC SLOTS
**
***************************************/

void Tree::show() {
  positionsInView();
  QAbstractItemView::show();
}
