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

#ifndef MARBABSTRACTITEMVIEW_H
#define MARBABSTRACTITEMVIEW_H

#include <QAbstractItemView>
#include <QPen>
#include <QPointer>
#include <QRubberBand>

class MarbItemDelegate;

class MarbAbstractItemView : public QAbstractItemView {
  Q_OBJECT
protected:
  QMap<QModelIndex, QPointF> myItemPos;
  QPointer<MarbItemDelegate> myDelegate;
  QRubberBand myRubberBand;

  QList<int> calculateColumnsOrder() const;

  virtual void updateValues() = 0;

  virtual void setScrollBarValues() = 0;
  QModelIndex moveCursor( QAbstractItemView::CursorAction cursorAction,
                          Qt::KeyboardModifiers modifiers );

  void resizeEvent( QResizeEvent* event );
public:
  explicit MarbAbstractItemView( QWidget* parent = 0 );

  int         horizontalOffset() const;
  int         verticalOffset() const;

  bool        isIndexHidden( const QModelIndex& index ) const;
  void        setSelection( const QRect&, QItemSelectionModel::SelectionFlags command );
  void        scrollTo( const QModelIndex& index, ScrollHint hint = EnsureVisible );
  QRect       visualRect( const QModelIndex& index ) const;
  QRegion     visualRegionForSelection( const QItemSelection& selection ) const;

  virtual QModelIndex indexAt(const QPoint &point) const;
  virtual QRectF itemRect( const QModelIndex& index ) const;
  virtual QPainterPath itemPath( const QModelIndex& index ) const = 0;

  int rows( const QModelIndex& index ) const;

  virtual void setModel( QAbstractItemModel* model );

protected slots:
  void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
  void rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end);
  void rowsInserted(const QModelIndex &parent, int start, int end);


public slots:
  virtual bool save( QString filename ) = 0;
};

#endif // MARBABSTRACTITEMVIEW_H
