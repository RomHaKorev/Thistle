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
#include "abstractitemview.h"
#include "itemdelegate.h"
#include "global.h"
#include <QScrollBar>
#include <QPainter>
#include <QPaintEvent>
#include <QStandardItem>

#include <QDebug>


#include "abstractitemview_p.h"

namespace Thistle {


  AbstractItemView::AbstractItemView(QWidget *parent) : QAbstractItemView( parent ) {
    d_ptr = new AbstractItemViewPrivate( this );
    setItemDelegate( d_ptr->delegate );
  }


  AbstractItemView::AbstractItemView( AbstractItemViewPrivate* d, QWidget* parent ) : QAbstractItemView( parent ), d_ptr( d ) {
    setItemDelegate( d_ptr->delegate );
  }

  AbstractItemView::~AbstractItemView() {
    delete d_ptr;
  }


  QMargins AbstractItemView::contentsMargins() const {
    return d_ptr->margins;
  }


  QRect AbstractItemView::contentsRect() const {
    return QRect( d_ptr->margins.left(), d_ptr->margins.top(), width() - d_ptr->margins.left() - d_ptr->margins.right(),
                  height() - d_ptr->margins.top() - d_ptr->margins.bottom() );
  }


  void AbstractItemView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
    QAbstractItemView::dataChanged(topLeft, bottomRight);
    update( this->model()->index(0, 0) );
  }


  int AbstractItemView::horizontalOffset() const {
    return horizontalScrollBar()->value();
  }


  QModelIndex AbstractItemView::indexAt(const QPoint &point) const {
    QPoint p = point    + QPoint( horizontalOffset(), verticalOffset() );
    if ( this->model() == 0 ) {
      return QModelIndex();
    }

    Q_FOREACH( QModelIndex index, d_ptr->itemPos.keys() ) {
      QPainterPath path = this->itemPath( index );
      if ( path.contains( p ) ) {
        return index;
      }
    }
    return QModelIndex();
  }


  bool AbstractItemView::isIndexHidden(const QModelIndex& /*index*/ ) const {
    return false;
  }

  QRectF AbstractItemView::itemRect( const QModelIndex& index ) const {
    return this->itemPath( index ).boundingRect();
  }

  QRectF AbstractItemView::itemRect( int row, int column, const QModelIndex& parent ) const {
    return this->itemRect( this->model()->index( row, column, parent ) );
  }

  QModelIndex AbstractItemView::moveCursor( QAbstractItemView::CursorAction cursorAction,
                                            Qt::KeyboardModifiers /*modifiers*/ ) {
    switch( cursorAction ) {
    case QAbstractItemView::MoveDown:
      break;
    default:
      break;
    }
    return QModelIndex();
  }


  void AbstractItemView::resizeEvent( QResizeEvent* event ) {
    setScrollBarValues();
    QAbstractItemView::resizeEvent( event );
  }


  int AbstractItemView::rows(const QModelIndex &index) const {
    return this->model()->rowCount(model()->parent(index));
  }


  void AbstractItemView::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end) {
    QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
    updateValues();
    viewport()->update();
    setScrollBarValues();
  }


  void AbstractItemView::rowsInserted(const QModelIndex& /*parent*/, int /*start*/, int /*end*/) {
    updateValues();
    viewport()->update();
    setScrollBarValues();
  }


  void AbstractItemView::scrollTo(const QModelIndex& index, ScrollHint hint ) {
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


  void AbstractItemView::setModel(QAbstractItemModel *model) {
    QAbstractItemView::setModel( model );
    this->updateValues();
  }


  void AbstractItemView::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command ) {
    QRect contentsRect = rect.translated(
          this->horizontalScrollBar()->value(),
          this->verticalScrollBar()->value()).normalized();
    int rows = this->model()->rowCount( this->rootIndex() );
    int columns = this->model()->columnCount( this->rootIndex() );
    int count = 0;
    QPainterPath contentsPath;
    contentsPath.addRect( contentsRect );
    for ( int row = 0; row < rows; ++row ) {
      for( int col = 0; col < columns; ++col ) {
        QModelIndex index = this->model()->index( row, col, this->rootIndex() );
        QPainterPath path = this->itemPath( index );
        if ( !path.intersected(contentsPath).isEmpty() ) {
          count += 1;
          this->selectionModel()->select( index, command );
        }
      }
    }
    if ( count == 0 ) {
      this->selectionModel()->clear();
    }
    this->viewport()->update();
  }


  int AbstractItemView::verticalOffset() const {
    return verticalScrollBar()->value();
  }


  QRect AbstractItemView::visualRect(const QModelIndex &index) const {
    QRect rect = itemRect(index).toRect();
    return rect;
  }


  QRegion AbstractItemView::visualRegionForSelection( const QItemSelection& selection ) const {
    Q_UNUSED( selection )
    return QRegion( QRect( 0, 0, this->width(), this->height() ) );
  }

}
