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

#ifndef PIECHART_H
#define PIECHART_H

#include <QAbstractItemView>

class PieChart : public QAbstractItemView {
  Q_OBJECT

public:
  explicit PieChart(QWidget *parent = 0);

  QRect visualRect(const QModelIndex &index) const;
  void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
  QModelIndex indexAt(const QPoint &point) const;

  void setRing( bool ring = true );
  void setSplitted( bool splitted = true );
  void setLegend( QString legend );
  virtual bool save( QString filename );

protected slots:
  void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
  void rowsInserted( const QModelIndex& parent, int start, int end );
  void rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end );

protected:
  QRect   myRect;
  qreal   myTotal;
  bool    mySplitted;
  bool    myRing;
  qreal   myStartAngle;
  QString myLegend;
  virtual void updateChart();
  virtual QPainterPath itemPath( const QModelIndex& index ) const;
  virtual QPainterPath itemPart( qreal angle, qreal delta, bool splitted = false ) const;
  virtual QPointF splittedOffset( qreal angle, qreal delta ) const;

  QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                         Qt::KeyboardModifiers modifiers);

  int horizontalOffset() const;
  int verticalOffset() const;

  bool isIndexHidden(const QModelIndex &index) const;

  void paintEvent(QPaintEvent *event);
  virtual void paintChart( QPainter& painter );
  virtual void paintPart( QPainter& painter, qreal angle, qreal delta, QColor color, bool isSelected = false );
  virtual void paintPartSplitted( QPainter& painter, qreal angle, qreal delta, QColor color, bool isSelected = false );
  virtual void configureColor( QPainter& painter, QColor base, int flag ) const;



  QRegion visualRegionForSelection(const QItemSelection &selection) const;
  void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
};

#endif // PIECHART_H
