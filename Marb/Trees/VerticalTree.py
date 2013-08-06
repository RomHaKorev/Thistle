from .Tree import Tree

import math
from PySide.QtCore import QPointF, Qt, QSize
from PySide.QtGui import QPixmap, QPainter


class VerticalTree(Tree):
		_directLink = False
		
		def __init__(self, parent=None):
				super(VerticalTree, self).__init__( parent )


		# Set the connection style
		# TODO : setDirectionStyle					
		def setDirectConnection(self, direct = True):
				self._directLink = direct
		 

		def setScrollBarValues(self):
				dw = max( [0, ( (self._realSize.width() + self._rect.width() / 2 ) - self.width()) + 20] )
				dh = max( [0, (self._realSize.height() + self._rect.height()/2 - self.height()) + 20 ] )
				self.horizontalScrollBar().setRange( 0, dw )
				self.verticalScrollBar().setRange( 0, dh )
				self.itemOffset = QPointF( 10, 10 )
				delta = self.width() - self._realSize.width()
				if delta > 20:
					self.itemOffset = QPointF( delta/2.0, 10 )
					


		def scan(self, index, left, depth):
				rows = self.model().rowCount( index )
				if not index.isValid():
						return 0
				elif rows == 0:
						self.setX( index, left )
						self.setY(index, depth)
						self._treeWidth += 1
						return (left + 1, 1)
				
				child_depth = 0
				for r in range( 0, self.model().rowCount( index ) ):
						child = index.child( r, 0 )
						(left, d) = self.scan( child, left, depth + 1 )
						child_depth = max( child_depth, d )
				
				left = self._itemTreePos[ self.model().index(0, index.column(), index) ].x()
				right = self._itemTreePos[ self.model().index( rows - 1, index.column(), index ) ].x()
				if rows >= 2:
						if rows % 2 == 1:
								r = math.floor( rows/2 ) + 1
								v = self._itemTreePos[ self.model().index( r - 1, index.column(), index ) ].x()
								self.setX( index, v )
						else:
								self.setX( index, (right + left) / 2 )
				else:
						self.setX( index, left )
						
				self.setY(index, depth)
				return ( right + 1 , child_depth + 1 ) 
						
				
		def _positionsInTree(self):
				self._itemTreePos = {}
				for row in range( self.model().rowCount() ) :
					(self._left, self._depth) = self.scan( self.model().index(row,0) , self._treeWidth, 0)
					self._left += 1
				self._left = 0
				for p in self._itemTreePos.values():
						self._left = max( self._left, p.x() )
				self._depth -= 1				
				self._positionsInView()


		def _positionsInView(self):
				self._realSize.setWidth( ( self._left ) * ( self._xDistance + self._rect.width() ) + self._rect.width() )
				self._realSize.setHeight( self._depth * ( self._yDistance + self._rect.height() ) + self._rect.height() )
				 
				self.setScrollBarValues()
				
				offset = QPointF( self._rect.width()/2, self._rect.height()/2 ) + QPointF(20, 20)
				for index in self._itemTreePos.keys():
						p = self._itemTreePos[ index ]
						self._itemPos[index] = QPointF( p.x() * ( self._xDistance + self._rect.width() ), p.y() * ( self._yDistance + self._rect.height() ) ) + offset
		
		
		def paintConnectionsFor(self, painter, index, offset ):
				painter.save()
				painter.setPen( self._connectionPen )

				if self._directLink == True:
						super(VerticalTree, self).paintConnectionsFor( painter, index, offset )

				else:
						rows = self.model().rowCount( index )
						r = self.itemRect(index).translated( offset.x(), offset.y() )
						if rows > 1:
								child1 = self.itemRect( self.model().index( 0, 0, index ) ).translated( offset.x(), offset.y() )
								child2 = self.itemRect( self.model().index( rows - 1, 0, index ) ).translated( offset.x(), offset.y() )
								p1 = QPointF( child1.center().x(), child1.top() - self._yDistance / 2 )
								p2 = QPointF( child2.center().x(), child2.top() - self._yDistance / 2 )
								painter.drawLine( p1, p2 )

								p1 = QPointF( r.center().x(), r.bottom() + 1 )
								p2 = QPointF( r.center().x(), r.bottom() + self._yDistance / 2 )
								painter.drawLine( p1, p2 )

								for i in range( 0, self.model().rowCount(index)):
										r = self.itemRect( self.model().index( i, 0, index ) ).translated( offset.x(), offset.y() )
										p1 = QPointF( r.center().x(), r.top() )
										p2 = QPointF( r.center().x(), r.top() - self._yDistance / 2 )
										painter.drawLine( p1, p2 )
										
						elif rows == 1:
								p1 = QPointF( r.center().x(), r.bottom() + 1 )
								p2 = QPointF( p1.x(), p1.y() + self._yDistance - 2 )
								painter.drawLine( p1, p2 )

				painter.restore()


		def save(self, filename ):
				s = self._realSize + QSize( 20, 20 )
				pix = QPixmap( s )
				pix.fill( Qt.transparent )
				painter = QPainter( pix )
				painter.setRenderHint( QPainter.Antialiasing )
				x = 10
				y = 10
				self.paintConnections( painter, QPointF( x, y ) )
				self.paintItems(painter , QPointF( x, y ) )
				painter.end()
				return pix.save( filename )


