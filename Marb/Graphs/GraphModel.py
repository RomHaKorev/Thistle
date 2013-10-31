from PySide.QtCore import QSize, QRect, QPoint, QPointF, Qt, QModelIndex
from PySide.QtGui import QStandardItemModel

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


class GraphModel( QStandardItemModel ):
	def __init__( self, *args, **kargs ):
		super( GraphModel, self ).__init__( *args, **kargs )
		self._edges = []


	def addEdgeByIndex( self, idx1, idx2, type = Edge.NoArrow ):
		''' Add an edge between the two nodes referenced by the QModelIndex idx1 and idx2.
		*idx1: QModelIndex
		*idx2: QModelIndex
		*type: Edge.type
		'''
		if ( idx1 == idx2 ) or not idx1.isValid() or not idx2.isValid():
			return None
		self._edges.append( Edge( idx1, idx2, type ) )


	def addEdge( self, row1, col1, row2, col2, type = Edge.NoArrow ):
		''' Add an edge between the two nodes referenced by the coordinates ([row1], [col1]) and ([row2], [col2]).
		*row1: integer
		*col1: integer
		*row2: integer
		*col2: integer
		*type: Edge.type
		'''
		self.addEdgeByIndex( self.index( row1, col1 ), self.index( row2, col2 ), type )

	def edges( self ):
		return self._edges