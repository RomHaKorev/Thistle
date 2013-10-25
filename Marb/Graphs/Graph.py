from PySide.QtGui import QPen, QStyledItemDelegate, QBrush, QFont, QPolygon, \
QLineEdit, QRegion, QColor, QPainter, QStyleOptionViewItem, QPolygonF
from PySide.QtCore import QSize, QRect, QPoint, QPointF, Qt, QModelIndex, QTimer, QLineF

from ..MarbItemDelegate import MarbItemDelegate
from ..MarbAbstractItemView import MarbAbstractItemView
from ..Global import Color, Shape
import math

import random

Pi = 3.14159265358979323846264338327950288419717

class Edge:
	'''The Edge class represents an edge between two nodes.
	An edge can be Bilateral, unilateral or None. The type of edge defines the kind of forces and the render.
	'''
	NoArrow = 0
	Bilateral = 1
	Unilateral = 2
	def __init__( self, idx1, idx2, type ):
		self.leftIndex = idx1
		self.rightIndex = idx2
		self.type = type
		

class Node:
	''' The class Node contains the specification for a particular node.
	It contains the edges and the position.
	'''
	def __init__(self):
		self._pos = QPointF( 0, 0 )
		self._connectedNode = []
	
	def pos(self):
		return self._pos
	
	
	def setPos(self, p):
		self._pos = p


	def addEdge( self, node ):
		if node != self:
			self._connectedNode.append( node )
 

	def edges( self ):
		return self._connectedNode


class Graph(MarbAbstractItemView):					
	def __init__( self, parent=None ):
		'''
		'''
		super( Graph, self ).__init__( parent )
		self._timer = QTimer()
		self._timer.setInterval(1)
		self._timer.timeout.connect( self.processTimer )
		#connect( &self.timer, SIGNAL(timeout()), self, SLOT(processTimer()) )
		delegate = MarbItemDelegate( self )
		style = delegate.itemStyle()
		style.setShape( Shape.Ellipse )
		delegate.setItemStyle( style )
		self.setItemDelegate( delegate )
		self._movableItem = True
		self._elasticItem = True
		self._weight = 9.81
		self._edges = []
		self._movedItem = None
		self._oldLength = 0
		self._rect = QRect( -20, -20, 40, 40 )
		self._oldPosMovedItem = QPointF()# Used to check if the node has been moved or just clicked.
		
		self._connectionPen = QPen( QColor( Color.Gray ), 3 )


	def setWeight(self, weight):
		'''Sets node weight to [weight].
		Weight will be used to define forces applied between each node.
		*weight: integer
		'''
		self._weight = weight

	def setItemSize(self, size ):
		'''Sets the item size to size.
		'''
		self._rect = QRect( -size.width()/2, -size.height()/2, size.width(), size.height() )
		self.viewport().update()

	def setConnectionPen(self, pen):
		''' Sets the Qpen used to paint the node edge to [pen].
		*pen: QPen
		'''
		self._connectionPen = pen

	def itemRect( self, index ):
		if not index.isValid():
			return QRect()
		pos = self._itemPos[ index ].pos()
		return self._rect.translated( pos.x(), pos.y() )


	def addEdgeByIndex( self, idx1, idx2, type = Edge.NoArrow ):
		''' Add an edge between the two nodes referenced by the QModelIndex idx1 and idx2.
		*idx1: QModelIndex
		*idx2: QModelIndex
		*type: Edge.type
		'''
		if ( idx1 == idx2 ) or not idx1.isValid() or not idx2.isValid():
			return None
		self._edges.append( Edge( idx1, idx2, type ) )
		self.updateValues()


	def addEdge( self, row1, col1, row2, col2, type = Edge.NoArrow ):
		''' Add an edge between the two nodes referenced by the coordinates ([row1], [col1]) and ([row2], [col2]).
		*row1: integer
		*col1: integer
		*row2: integer
		*col2: integer
		*type: Edge.type
		'''
		if self.model() == None:
			return
		self.addEdgeByIndex( self.model().index( row1, col1 ), self.model().index( row2, col2 ), type )



	def updateValues( self ):
		if self.model() == 0:
			return None
		self._timer.stop()
		self._itemPos = {}
		rows = self.model().rowCount()
		nbRows = int( math.sqrt( rows ) )

		x = 10
		y = 10
		i = 0
		for r in range( rows ):
			idx = self.model().index( r, 0 )
			if idx.isValid() and idx.data() != None:
				n = Node()
				n.setPos( QPointF( x, y ) )
				self._itemPos[ idx ] = n
				x += 100
				i += 1
				if i > nbRows:
					x = 10
					y += 100
					i = 0
		
		for e in self._edges:
			self._itemPos[ e.leftIndex ].addEdge( ( self._itemPos[ e.rightIndex ] ) )
			self._itemPos[ e.rightIndex ].addEdge( ( self._itemPos[ e.leftIndex ] ) )
			
		self._timer.start()
		oldLength = 0


	def shake(self): # Shiver and Moan...
		''' Shakes nodes to place them at a random position.
		'''
		if self.model() == 0:
			return None
		self._timer.stop()
		self._itemPos = {}
		rows = self.model().rowCount()

		for r in range( rows ):
			idx = self.model().index( r, 0 )
			if idx.isValid() and idx.data() != None:
				x = random.randint( 0, self.width() )
				y = random.randint( 0, self.height() )
				n = Node()
				n.setPos( QPointF( x, y ) )
				self._itemPos[ idx ] = n
						
		for e in self._edges:
			self._itemPos[ e.leftIndex ].addEdge( ( self._itemPos[ e.rightIndex ] ) )
			self._itemPos[ e.rightIndex ].addEdge( ( self._itemPos[ e.leftIndex ] ) )
			
		self._timer.start()
		oldLength = 0
		

	def paintEvent( self, event ):
		painter = QPainter(self.viewport())
		painter.setRenderHint( QPainter.Antialiasing )
		self.paintEdges( painter )
		self.paintItems( painter )


	def paintEdges( self, painter, offset = QPointF( 0, 0 ) ):
		''' Paints the edges between nodes on the paint device [painter] with the given [offset].
		*painter: QPainter
		*offset: QPointF
		'''
		painter.save()
		painter.setPen( self._connectionPen )
		painter.setBrush( QColor( Color.Gray ) )
		for edge in self._edges:
			self.paintEdge( painter, edge.leftIndex, edge.rightIndex, edge.type )
		painter.restore()
	
	
	def paintEdge( self, painter, idx1, idx2, type ):
		''' Paints the edge between the two nodes referenced by the QModelIndex [idx1] and [idx2] on the paint device [painter] with the given [offset].
		*painter: QPainter
		*idx1: QModelIndex
		*idx2: QModelIndex
		*type: Edge.type
		'''
		r1 = self.itemRect( idx1 )
		r2 = self.itemRect( idx2 )
		p1 = r1.center()
		p2 = r2.center()
		line = QLineF( p1, p2 )
		
		if type == Edge.NoArrow:
			painter.drawLine( line )
			return None

		i = 0
		p = QPointF()
		l = [ r1.topLeft(), r1.topRight(), r1.bottomRight(), r1.bottomLeft(), r1.topLeft() ]
		for i in range( l.size() - 1 ):
			type, p = line.intersect( QLineF( l[i], l[i + 1] ), p )
			if type == QLineF.BoundedIntersection:
				p1 = p
				break

		l = [ r2.topLeft(), r2.topRight(), r2.bottomRight(), r2.bottomLeft(), r2.topLeft() ]
		for i in range( l.size() - 1 ):
			type, p = line.intersect( QLineF( l[i], l[i + 1] ), p )
			if type == QLineF.BoundedIntersection:
				p2 = p
				break
		
		line = QLineF( p1, p2 )
		if type == Edge.Bilateral:
			painter.drawLine( line.pointAt( 0.1 ), line.pointAt( 0.9 ) )
			self.paintArrow( painter, QLineF( p1, p2 ) )
			self.paintArrow( painter, QLineF( p2, p1 ) )
		else:
			painter.drawLine( line.p1(), line.pointAt( 0.9 ) )
			self.paintArrow( painter, QLineF( p1, p2 ) )


	def paintArrow( self, painter, line ):
		''' Paints an arrow the [line] end on the paint device [painter].
		*line: QLineF
		*painter: QPainter
		'''
		originalPen = painter.pen()
		pen = painter.pen()
		pen.setWidth( 1 )
		painter.setPen( pen )
		p1 = line.p2()
		l = QLineF( p1, line.pointAt( (line.length() - 15)/line.length() ) )
		l.setAngle( l.angle() + 30 )
		p2 = l.p2()
		l = QLineF( p1, line.pointAt( (line.length() - 15)/line.length() ) )
		l.setAngle( l.angle() - 30 )
		p3 = l.p2()
		painter.drawPolygon( QPolygonF() << p1 << p2 << p3 )
		painter.setPen( originalPen )


	def paintItems( self, painter, offset = QPointF(0,0) ):
		''' Paints every node on the paint device [painter] with the given [offset].
		*painter: QPainter
		*offset: QPointF
		'''
		for idx in self._itemPos.keys():
			option = QStyleOptionViewItem()
			option.rect = self.itemRect( idx ).translated( offset.x(), offset.y() )
			self.itemDelegate().paint( painter, option, idx )


	def calculateForces( self, idx ):
		''' calculateForces applied to the node referenced by the QModelIndex [idx]
		* idx: QModelIndex
		'''
		node = self._itemPos[ idx ]
		
		# Calculate forces pushing the items
		velocity = QPointF( 0, 0 )
		for n in self._itemPos.values():
			vec = QPointF( node.pos() - n.pos() )
			dx = vec.x()
			dy = vec.y()
			l = 2.0 * float(dx * dx + dy * dy)
			if (l > 0):
				velocity += QPointF( (dx * 500.0) / l, (dy * 500.0) / l )

		#Calculate forces pulling items relative to their weight
		weight = ( len(node.edges() ) + 1 ) * self._weight
		for n in node.edges():
			vec = QPointF( node.pos() - n.pos() )
			velocity -= QPointF( vec.x() / weight, vec.y() / weight )
		
		# Do not apply velocity if it's too low
		if (abs(velocity.x()) < 0.1) and (abs(velocity.y()) < 0.1):
			velocity = QPointF( 0, 0 )
		
		if idx != self._movedItem:
			node.setPos( node.pos() + velocity )


	def processTimer( self ):
		''' Processes a single step of the algorithm.
		'''
		if self._itemPos == {}:
			self._movedItem = QModelIndex()
			self.timer.stop()
			return

		for idx in self._itemPos.keys():
			self.calculateForces( idx )
			self.calculateForces( idx )

		poly = QPolygonF()
		for n in self._itemPos.values():
			poly.append( n.pos() )

		offset = QPointF( 50, 50 ) - poly.boundingRect().topLeft()
		for idx in self._itemPos.keys():
			self._itemPos[ idx ].setPos( self._itemPos[ idx ].pos() + offset )

		self._realSize = poly.boundingRect().size() + 2 * ( self.itemRect( self.model().index( 0, 0 ) ).size() )
		
		self.viewport().update()
		length = 0
		
		pts = []
		
		for n in self._itemPos.values():
			pts.append( n.pos() )

		p1 = pts[0]
		for i in range( 1 ,len( pts ) ):
			length += QLineF( p1, pts[i] ).length()
			p1 = pts[i]

		if abs(length - self._oldLength ) > 0.0001:
			self._oldLength = length
		else:
			self._timer.stop()


	def mousePressEvent(self, event ):
		if self._movableItem == True:
			idx = self.indexAt( event.pos() )
			if idx.isValid():
				self._movedItem = idx
				self._oldPosMovedItem = self._itemPos[ idx ].pos()
		super(Graph, self).mousePressEvent( event )
	
	
	def mouseReleaseEvent(self, event ):
		super(Graph, self).mouseReleaseEvent( event )
		if self._movableItem == True:
			if self._elasticItem == True:
				self._timer.start()
			else:
				self._movedItem = QModelIndex()


	def mouseMoveEvent(self, event ):
		if self._movableItem == True and self._movedItem.isValid():
			self._itemPos[ self._movedItem ].setPos( QPointF(event.pos()) )
		super(Graph, self).mouseMoveEvent( event )
		self.viewport().update()

