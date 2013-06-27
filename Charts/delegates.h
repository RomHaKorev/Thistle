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

#ifndef DELEGATES_H
#define DELEGATES_H
#include <QStyledItemDelegate>

class Chart;

class PointDelegate : public QStyledItemDelegate {
protected:
  Chart* myParent;
  QPolygon createDiamond( QRect rect) const;
public:
  explicit PointDelegate( Chart* parent = 0 );
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class BarDelegate : public QStyledItemDelegate {
protected:
  Chart* myParent;
public:
  explicit BarDelegate( Chart* parent = 0 );
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



/* def paint( self, painter, option, index ):



class BarDelegate( QStyledItemDelegate ):
  def __init__( self, parent ):
    super(BarDelegate, self).__init__( parent )

  def paint( self, painter, option, index ):
    chartItemStyle = self.parent().columnStyle( index.column() )
    r = option.rect

    painter.save()

    painter.setBrush( chartItemStyle.brush() )
    painter.setPen( Qt.NoPen )
    painter.drawRect( r )

    polygon = QPolygon()
    if option.Position == QStyleOptionViewItem.Top: # Negative value
      polygon.append( r.bottomLeft() )
      polygon.append( r.topLeft() )
      polygon.append( r.topRight() )
      polygon.append( r.bottomRight() )
    else:
      polygon.append( r.topLeft() )
      polygon.append( r.bottomLeft() )
      polygon.append( r.bottomRight() )
      polygon.append( r.topRight() )

    painter.setBrush( Qt.NoBrush )
    pen = QPen(chartItemStyle.pen())
    pen.setCapStyle( Qt.FlatCap )
    painter.setPen( pen )

    painter.drawPolyline( polygon )

    painter.restore()*/

#endif // DELEGATES_H
