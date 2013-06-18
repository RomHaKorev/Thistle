
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



from .Tree import Tree, TreeItemDelegate
from .Tree import TreeStyle
from ..Global import Shape

import math
from PySide.QtGui import QPainterPath, QPainter, QPen, QPixmap
from PySide.QtCore import QRect, QPointF, Qt, QSize


class RadialTree(Tree):
		def __init__(self, parent=None):
				super(RadialTree, self).__init__( parent )
				self._rotateText = False
				self.orderedIndexes = []
				self._rect = QRect( -20, -20, 40, 40 )
				style = TreeStyle()
				style.setShape( Shape.Ellipse )
				
				delegate = TreeItemDelegate( self )
				delegate.setTreeStyle( style )
				self.setItemDelegate( delegate )
				
				self._yDistance = 40
				self._xDistance = 40
				
				self.radius = 1
				self.diagonal = 1
				
				self._displayCircle = True
				
		
		def displayCicrcle(self, display = True ):
				self._displayCircle = display
				
				
		def setScrollBarValues(self):
				s = self._realSize + QSize( 50, 50 )
				dw = max( 0, ( s.width() - self.width() )/2	)
				dh = max ( 0, ( s.height() - self.height() )/2 )
				self.horizontalScrollBar().setRange( -dw, dw )
				self.verticalScrollBar().setRange( -dh, dh )
				self.itemOffset = QPointF( self.width() / 2, self.height() / 2 )
				
				
		def updatePerimeter(self):
				w = self._rect.width() + self._xDistance
				h = self._rect.height() + self._yDistance
				self.diagonal = math.sqrt( w * w + h * h )
				self.perimeter = 0.0
				factor = 1
				for index in self._itemTreePos.keys():
						if self.model().rowCount( index ) == 0:
								factor = self._depth / self._itemTreePos[index].y()
								self.perimeter += self.diagonal * factor
				
				self.radius = self.perimeter / ( 2 * math.pi )
				if self.radius * 2 < self.diagonal * self._depth:
						self.radius = (self.diagonal * self._depth)/2 * 1.5
		
		
		def scan(self, index, _left, _depth):
				self.orderedIndexes.append( index )
				rows = self.model().rowCount( index )
				if not index.isValid():
						return 0
				elif rows == 0:
						self.setX( index, _left )
						self.setY(index, _depth + 1)
						return (_left + 1, 1)
				
				child_depth = 0
				for r in range( 0, self.model().rowCount( index ) ):
						child = index.child( r, 0 )
						(_left, d) = self.scan( child, _left, _depth + 1 )
						child_depth = max( child_depth, d )
				
				_left = self._itemTreePos[ self.model().index(0, index.column(), index) ].x()
				right =	self._itemTreePos[ self.model().index( rows - 1, index.column(), index ) ].x();
				if rows >= 2:
						if rows % 2 == 1:
								r = math.floor( rows/2 ) + 1
								v = self._itemTreePos[ self.model().index( r - 1, index.column(), index ) ].x()
								self.setX( index, v )
						else:
								self.setX( index, float(right + _left + 1) / 2.0 )
				else:
						self.setX( index, _left )

				self.setY(index, _depth )
				return ( right + 1 , child_depth + 1 ) 
				
		
		def _positionsInTree(self):
				self._itemTreePos = {}
				self.orderedIndexes = []
				
				(self._left, self._depth) = self.scan( self.model().index(0,0) , 0, 0)
				self._left += 1

				self._positionsInView()
				
				
		def _positionsInView(self):
				if len(self._itemTreePos.keys()) == 0:
						return None
				
				self.updatePerimeter()
				
				self._realSize.setWidth( self.radius * 2 + self._rect.width())
				self._realSize.setHeight( self.radius * 2 + self._rect.height() )
				self.setScrollBarValues()
				
				self._itemPos = {}
				
				for index in self._itemTreePos.keys():
						p = self._itemTreePos[ index ]
						self._itemPos[ index ] = QPointF( p.x(), p.y() )
				
				path = QPainterPath()
				path.addEllipse( self.itemOffset + self._rect.center(), self.radius, self.radius )
				
				l = 0
				factor = 0
				for index in self.orderedIndexes:
						if self.model().rowCount( index ) != 0:
								continue
						
						factor = float(self._depth) / float(self._itemPos[index].y())
						
						if self.model().rowCount( index ) == 0:
								l +=	(float(self.diagonal)/2.0) * float(factor)
						
						percent = path.percentAtLength( l )
						self._itemPos[index] = QPointF( percent, self._itemPos[index].y() )

						if self.model().rowCount( index ) == 0:
								l +=	(float(self.diagonal)/2.0) * float(factor)
											 
				self.orderedIndexes.reverse()
				for index in self.orderedIndexes:
						if self.model().rowCount( index ) == 0:
								continue
						_left = self.model().index( 0,0, index )
						right = self.model().index( self.model().rowCount(index) - 1,0, index )
						self._itemPos[index] = QPointF( float(self._itemPos[_left].x() + self._itemPos[right].x()) / 2.0, self._itemPos[index].y() )				
				
				self._itemPos[self.model().index(0,0)] = QPointF(0, 0)
				
				self.orderedIndexes.reverse()
				
				for index in self.orderedIndexes:
						if self._depth == self._itemPos[index].y():
								factor = 1.0
						else:
								if self._itemPos[index].y() != 0:
										factor = float(self._depth) / float(self._itemPos[index].y())
								else:
										factor = 1.0

						radius = float(self.radius) / float(factor)
						circle = QPainterPath()
						circle.addEllipse( self._rect.center(), radius, radius )
						
						self._itemPos[index] = circle.pointAtPercent( self._itemPos[index].x() )
						
				self._itemPos[self.model().index(0,0)] = QPointF(0, 0)

				
		def paintConnections(self, painter, offset):
				if self._displayCircle == True:
						self.paintCircles(painter, offset)

				Tree.paintConnections(self, painter, offset)

				
		def paintCircles(self, painter, offset):
				painter.save()
				c = self._connectionPen.color()
				c.setAlpha( c.alpha() * 0.15 )
				pen = QPen(self._connectionPen)
				pen.setColor( c )
				pen.setWidth( pen.width() * 2 )
				painter.setPen( pen )
				painter.setRenderHint( QPainter.Antialiasing )
				index = self.model().index(0,0)
				center = self.itemRect(index).translated( offset.x(), offset.y() ).center()
				for rd in range( 1, int(self._depth) + 1 ):
						r = float(self.radius) * rd / self._depth
						painter.drawEllipse( center, r, r )
				painter.restore()
						
		def save(self, filename ):
				s = self._realSize + QSize( 20, 20 )
				pix = QPixmap( s )
				pix.fill( Qt.transparent )
				painter = QPainter( pix )
				painter.setRenderHint( QPainter.Antialiasing )
				x = self._realSize.width()/2 + 10
				y = self._realSize.height()/2 + 10
				self.paintConnections( painter, QPointF( x, y ) )
				self.paintItems(painter , QPointF( x, y ) )
				painter.end()
				return pix.save( filename )
