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

#include "abstractmarbview.h"
#include <QFontMetrics>
#include <qmath.h>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QDebug>

AbstractMarbView::AbstractMarbView( QWidget* parent ) : QAbstractItemView( parent ) {
}

AbstractMarbView::~AbstractMarbView() {

}

int AbstractMarbView::horizontalOffset() const {
  return horizontalScrollBar()->value();
}

QModelIndex AbstractMarbView::indexAt(const QPoint& point) const {
  Q_UNUSED( point )
  return QModelIndex();
}

bool AbstractMarbView::isIndexHidden(const QModelIndex& /*index*/ ) const {
  return false;
}

QModelIndex AbstractMarbView::moveCursor( QAbstractItemView::CursorAction cursorAction,
                       Qt::KeyboardModifiers modifiers ) {
  Q_UNUSED( cursorAction )
  Q_UNUSED( modifiers )
/*
  int minRow = model()->rowCount() - 1;
  int minCol = model()->columnCount() - 1;
  int maxRow = 0;
  int maxCol = 0;

  foreach( QItemSelectionRange range, mySelections->selection() ) {
    minRow = qMin( minRow, range.bottom() );
    maxRow = qMax( maxRow, range.top() );
    minCol = qMin( minCol, range.left() );
    maxCol = qMax( maxCol, range.right() );
  }

  if ( cursorAction == MoveLeft ) {
    if ( minRow > 0 ) {
      --minRow;
    }
    if ( maxRow > 0 ) {
      --maxRow;
    }
  } else if ( cursorAction == MoveRight ) {
    if ( minRow < model()->rowCount() - 1 ) {
      ++minRow;
    }
    if ( maxRow < model()->rowCount() - 1 ) {
      ++maxRow;
    }
  }

  QItemSelection selection(
                            model()->index(minRow, minCol, rootIndex()),
                            model()->index(maxRow, maxCol, rootIndex())
                          );
  mySelections->select( selection, QItemSelectionModel::Clear | QItemSelectionModel::Select );
  viewport()->update();*/
  return QModelIndex();
}

int AbstractMarbView::rows( const QModelIndex& index ) const {
  return model()->rowCount( model()->parent( index ) );
}

void AbstractMarbView::scrollTo( const QModelIndex& /*index*/, ScrollHint /*hint*/ ) {
}

void AbstractMarbView::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command ) {
  Q_UNUSED( rect )
  Q_UNUSED( command )
}

int AbstractMarbView::verticalOffset() const {
  return verticalScrollBar()->value();
}

QRect AbstractMarbView::visualRect(const QModelIndex &index) const {
  QRectF rect = itemRect(index);
  if (rect.isValid())
      return QRect(rect.left() - horizontalScrollBar()->value(),
                   rect.top() - verticalScrollBar()->value(),
                   rect.width(), rect.height());
  else
      return rect.toRect();
}

QRegion AbstractMarbView::visualRegionForSelection(const QItemSelection& /*selection*/ ) const {
  return QRegion();
}


/***************************************
**
**        PROTECTED SLOTS
**
***************************************/

void AbstractMarbView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  QAbstractItemView::dataChanged(topLeft, bottomRight);
}

void AbstractMarbView::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end) {
  QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void AbstractMarbView::rowsInserted(const QModelIndex& parent, int start, int end) {
  QAbstractItemView::rowsInserted(parent, start, end);
}

void AbstractMarbView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
  QAbstractItemView::selectionChanged( selected, deselected );
}
