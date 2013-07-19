from PySide.QtGui import QPen, QStyledItemDelegate, QBrush, QFont, QPolygon, \
QLineEdit, QRegion, QColor, QPainter, QStyleOptionViewItem
from PySide.QtCore import QSize, QRect, QPoint, QPointF, Qt, QModelIndex

from ..MarbItemDelegate import MarbItemDelegate
from ..MarbAbstractItemView import MarbAbstractItemView
from ..Global import Color

class Tree(MarbAbstractItemView):					
		def __init__(self, parent=None):
				super(Tree, self).__init__( parent )
				self._realSize = QSize( 200, 200)
				self._rect = QRect( -40, -20, 80, 40 );
				self._yDistance = 50
				self._xDistance = 20
				self._depth = 0
				self._left = 0
				self._connectionPen = QPen( QColor(Color.LightGray), 2 )
				self.itemOffset = QPoint()
				delegate = MarbItemDelegate( self )
				self.setItemDelegate( delegate )
				self._itemPos = {}
				self._itemTreePos = {}	


		def setSpacing(self, horizontal, vertical ):
				self._xDistance = horizontal
				self._yDistance = vertical
				self._positionsInView()
				self.update()
				
						
		def setConnectionPen(self, pen ):
				self._connectionPen = pen
		
		
		def connectionPen(self):
				return self._connectionPen
		
		
		def scan(self, index, _left, _depth):
				raise( "Must be implemented. Should resolve the _left and _depth for each node." )
		
		
		def _positionsInTree(self):
				raise( "Must be implemented. Should resolve the position (x,y) in the tree for each node." )


		def _positionsInView(self):
				raise( "Must be implemented. Should resolve the position (x, y) in pixel in viewport for each node." )


		def updateValues(self):
			self._positionsInTree()
				

		def itemRect(self, index):
				if not index.isValid():
						return QRect()
				p	= self._itemPos[ index ] - QPointF( self.horizontalOffset(), self.verticalOffset() )
				return self._rect.translated( p.x(), p.y() )

		
		def setItemSize(self, size ):
				self._rect = QRect( -size.width()/2, -size.height()/2, size.width(), size.height() )
#				self._rect = r
				self._positionsInView()
				self.viewport().update()
		
								
		def setX(self, index, x):
				if not index in self._itemTreePos:
						self._itemTreePos[ index ] = QPointF()
				self._itemTreePos[index].setX(x)
		
		
		def setY(self, index, y):
				if not index in self._itemTreePos:
						self._itemTreePos[ index ] = QPointF()
						
				self._itemTreePos[index].setY(y)
				
				
		def paintEvent(self, event):
				painter = QPainter( self.viewport() )
				painter.setClipRect( event.rect() )
				painter.setRenderHint( QPainter.Antialiasing )
				self.paintConnections( painter, self.itemOffset )
				self.paintItems( painter, self.itemOffset )
			 
				
		def paintItems( self, painter, offset ):
				for index in self._itemPos.keys():
						option = QStyleOptionViewItem()
						option.rect = self.itemRect( index ).translated( offset.x(), offset.y() )
						self.itemDelegate().paint( painter, option, index )
		
		
		def paintConnections( self, painter, offset ):
				painter.save()
				painter.setPen( self._connectionPen )
				for index in self._itemPos.keys():
						self.paintConnectionsFor( painter, index, offset )
				painter.restore()
		
		
		def paintConnectionsFor( self, painter, index, offset ):
				painter.save()
				parent = self.model().parent( index )
				if parent.isValid():
						p1 = self.itemRect(index).translated( offset.x(), offset.y() ).center()
						p2 = self.itemRect(parent).translated( offset.x(), offset.y() ).center()
						painter.drawLine( p1, p2 )
				painter.restore()

