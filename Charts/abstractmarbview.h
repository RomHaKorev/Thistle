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

#ifndef ABSTRACTMARBVIEW_H
#define ABSTRACTMARBVIEW_H

#include <QAbstractItemView>

class AbstractMarbView : public QAbstractItemView {
  Q_OBJECT
public:
  explicit AbstractMarbView(QWidget *parent = 0);
  ~AbstractMarbView();

  virtual QRectF itemRect( const QModelIndex& index) const = 0;

  QModelIndex indexAt(const QPoint &point) const;
  void        setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);
  QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                         Qt::KeyboardModifiers modifiers);
  void         paintEvent(QPaintEvent *event) = 0;

  int         rows(const QModelIndex &index = QModelIndex()) const;

  void        scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);

  bool        isIndexHidden(const QModelIndex &index) const;

  int         horizontalOffset() const;
  int         verticalOffset() const;

  QRect       visualRect(const QModelIndex &index) const;
  QRegion     visualRegionForSelection(const QItemSelection &selection) const;
protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // ABSTRACTMARBVIEW_H
