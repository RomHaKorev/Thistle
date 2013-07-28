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

#ifndef LINEARCHART_H
#define LINEARCHART_H

#include "chart.h"
#include "chartstyle.h"

#include <QItemDelegate>

class PointDelegate;
class BarDelegate;

class LinearChart : public Chart {
  Q_OBJECT
public:
  explicit LinearChart(QWidget *parent = 0);
protected:
  PointDelegate* myPointDelegate;
  BarDelegate* myBarDelegate;
  bool myVerticalXAxis;

  QList<int> calculateColumnsOrder();
  virtual void updateValues();
  Marb::Types columnType( int column ) const;
  QList<int> barStyleColumns() const;
  QRectF itemRect( const QModelIndex& index ) const;
  void paintAxis( QPainter& painter );
  void paintXAxis( QPainter& painter );
  void paintYAxis( QPainter& painter );
  void paintTextAxis( QPainter& painter );
  void paintValues( QPainter& painter, int column);
  virtual void paintColumnLegend(QPainter &painter, int column, QPoint pos, int maxHeight);
  virtual void paintChart( QPainter& painter );
};
#endif // LINEARCHART_H
