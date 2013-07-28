#include "marbabstractitemview.h"

#include "marbitemdelegate.h"

#include <QScrollBar>
#include <QPainter>
#include <QPaintEvent>

#include <QStandardItem>

#include "Marb.h"

MarbAbstractItemView::MarbAbstractItemView(QWidget *parent) : QAbstractItemView( parent ) {
  myDelegate = new MarbItemDelegate( this );
  setItemDelegate( myDelegate );
}

void MarbAbstractItemView::setModel(QAbstractItemModel *model) {
  QAbstractItemView::setModel( model );
  updateValues();
}


int MarbAbstractItemView::horizontalOffset() const {
  return horizontalScrollBar()->value();
}


void MarbAbstractItemView::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command ) {
  QRect selectRect = rect;

  int rows = model()->rowCount(rootIndex());
  QModelIndexList indexes;

  for (int row = 0; row < rows; ++row) {
    QModelIndex index = model()->index(row, 0, rootIndex());
    if ( !itemRect( index ).intersected( selectRect ).isEmpty() ) {
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


bool MarbAbstractItemView::isIndexHidden(const QModelIndex& /*index*/ ) const {
  return false;
}


QModelIndex MarbAbstractItemView::moveCursor( QAbstractItemView::CursorAction cursorAction,
                       Qt::KeyboardModifiers /*modifiers*/ ) {
  switch( cursorAction ) {
  case QAbstractItemView::MoveDown:
  break;
  default:
  break;
  }

  return QModelIndex();
}


int MarbAbstractItemView::rows(const QModelIndex &index) const {
  return model()->rowCount(model()->parent(index));
}


void MarbAbstractItemView::scrollTo(const QModelIndex& index, ScrollHint hint ) {
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


int MarbAbstractItemView::verticalOffset() const {
  return verticalScrollBar()->value();
}


QRect MarbAbstractItemView::visualRect(const QModelIndex &index) const {
  QRect rect = itemRect(index).toRect();
  return rect;
}


QRegion MarbAbstractItemView::visualRegionForSelection(const QItemSelection &selection) const {
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


QRectF MarbAbstractItemView::itemRect(const QModelIndex &index) const {
  return QRectF();
}


/***************************************
**
**        PROTECTED SLOTS
**
***************************************/
void MarbAbstractItemView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  QAbstractItemView::dataChanged(topLeft, bottomRight);
  update( model()->index(0, 0) );
}

void MarbAbstractItemView::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end) {
  QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
  updateValues();
  viewport()->update();
  setScrollBarValues();
}

void MarbAbstractItemView::rowsInserted(const QModelIndex& /*parent*/, int /*start*/, int /*end*/) {
  updateValues();
  viewport()->update();
  setScrollBarValues();
}
