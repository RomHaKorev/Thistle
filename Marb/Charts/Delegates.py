
# This file is part of Marb.
# 
#     Marb is free software: you can redistribute it and/or modify
#     it under the terms of the Lesser GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License.
# 
#     Marb is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#     Lesser GNU General Public License for more details.
# 
#     You should have received a copy of the Lesser GNU General Public License
#     along with Marb.  If not, see <http://www.gnu.org/licenses/>.
# 
# Marb  Copyright (C) 2013  Dimitry Ernot



from ..Global import Shape

from PySide.QtGui import QPen, QStyledItemDelegate, QPolygon, QStyleOptionViewItem
from PySide.QtCore import QPoint, Qt


class PointDelegate( QStyledItemDelegate ):
	'''PointDelegate class provides display facilities for the Linear Chart.
	
	When displaying data from a linear Chart series configured with the flag Marb.Type.Point, the values are drawn with this delegate.
	'''
	def __init__( self, parent ):
		super(PointDelegate, self).__init__( parent )
		

	def _createDiamond(self, rect):
		''' Returns a polygon representing a diamond. The dimensions and the position are fixed by the QRect rect.
		
		The method is called when you use the flag Marb.Shape.Diamond
		'''
		poly = QPolygon()
		poly.append( rect.topLeft() + QPoint(rect.width()/2, 0) )
		poly.append( rect.topRight() + QPoint(0, rect.height()/2) )
		poly.append(rect.bottomLeft() + QPoint( rect.width()/2, 0 ))
		poly.append(rect.topLeft() + QPoint( 0, rect.height()/2 ))
		return poly
		
		
	def paint( self, painter, option, index ):
		'''Paints the data of the index relative to the shape set in the ChartStyle corresponding to the column of index.
		'''
		chartItemStyle = self.parent().columnStyle( index.column() )

		painter.save()
		painter.setBrush( chartItemStyle.brush() )
		painter.setPen( chartItemStyle.pen() )
		r = option.rect

		if chartItemStyle.shape() == Shape.Ellipse:
			painter.drawEllipse( r )
		elif chartItemStyle.shape() == Shape.RoundedRect:
			painter.drawRoundedRect( r, 5, 5 )
		elif chartItemStyle.shape() == Shape.Diamond:
			painter.drawPolygon( self._createDiamond( r ) )
		elif chartItemStyle.shape() == Shape.Triangle:
			poly = QPolygon()
			poly.append( r.topLeft() + QPoint( r.width()/2, 0 ) )
			poly.append( r.bottomLeft() )
			poly.append( r.bottomRight() )
			painter.drawPolygon( poly )
		elif chartItemStyle.shape() == Shape.ReversedTriangle:
			poly = QPolygon()
			poly.append( r.bottomLeft() + QPoint( r.width()/2, 0 ) )
			poly.append( r.topLeft() )
			poly.append( r.topRight() )
			painter.drawPolygon( poly )
		else:
			painter.drawRect( r )
		painter.restore()


class BarDelegate( QStyledItemDelegate ):
	'''BarDelegate class provides display facilities for the Linear Chart.
	
	When displaying data from a linear Chart series configured with the flag Type.Bar, the values are drawn with this delegate.
	'''
	def __init__( self, parent ):
		super(BarDelegate, self).__init__( parent )


	def paint( self, painter, option, index ):
		'''Paints the data of the index relative to the shape set in the ChartStyle corresponding to the column of index.
		'''
		chartItemStyle = self.parent().columnStyle( index.column() )
		r = option.rect

		painter.save()

		painter.setBrush( chartItemStyle.brush() )
		painter.setPen( Qt.NoPen )
		painter.drawRect( r )

		polygon = QPolygon()
		if option.decorationPosition == QStyleOptionViewItem.Top: # Negative value
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
		
		painter.restore()




