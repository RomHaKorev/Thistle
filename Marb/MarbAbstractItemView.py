from PySide.QtGui import QAbstractItemView
from PySide.QtCore import QModelIndex

class MarbAbstractItemView( QAbstractItemView ):
	def __init__(self, parent=None):
		super(MarbAbstractItemView, self).__init__( parent )

	def setScrollBarValues(self):
		None 


	def resizeEvent(self, ev ):
		self.setScrollBarValues()
		return QAbstractItemView.resizeEvent(self, ev )


	def moveCursor(self, cursorAction, modifiers):
		return QModelIndex()


	def itemRect(self, index):
		return QRectF()


	def horizontalOffset(self):
		return self.horizontalScrollBar().value()
	
	
	def verticalOffset(self):
		return self.verticalScrollBar().value()
		
			
	def indexAt(self, point):
		return QModelIndex() # BUG IN PySide
		p = point - self.itemOffset.toPoint() - QPoint( self.horizontalOffset(), self.verticalOffset() )
		for index in self._itemPos.keys():
				r = self.itemRect(index)
				if r.contains( p ):
						None
						return index

	
	def visualRect(self, index):
		r = self.itemRect(index).translated( self.itemOffset.x(), self.itemOffset.y() )
		return r
	
	
	def visualRegionForSelection(self, selection):
		ranges = selection.count()
		region = QRegion()
		for i in range(0, ranges):
				ran = self.selection.at(i)
				for row in range( ran.top(), ran.bottom() ):
						index = self.model().index( row, 0, self.rootIndex() )
						region += self.visualRect( index )
	
	
	def rows(self, index):
		return self.model().rowCount( self.model().parent(index))
	
	
	def setModel(self, *args, **kwargs):
		QAbstractItemView.setModel(self, *args, **kwargs)
		self.updateValues()

	
	def dataChanged(self, top_left, bottomRight ):
		QAbstractItemView.dataChanged(self, top_left, bottomRight)
		self.viewport().update()
	
			
	def rowsAboutToBeRemoved(self, parent, start, end):
		QAbstractItemView.rowsAboutToBeRemoved(self, parent, start, end)
		self.updateValues()
		self.viewport().update()
		self.setScrollBarValues()


	def rowsInserted(self, parent, start, end):
		QAbstractItemView.rowsInserted(self, parent, start, end)
		self.updateValues()
		self.viewport().update()
		self.setScrollBarValues()
	

	def updateValues( self ):
		raise( NotImplementedError )
		

	def paintEvent(self, event):
		raise( NotImplementedError )

	def save(self, filename ):
		raise( NotImplementedError )
