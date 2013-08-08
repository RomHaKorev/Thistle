from PySide.QtGui import QAbstractItemView, QTableView, QItemSelection, QRegion
from PySide.QtCore import QModelIndex, QPoint, QRect

class MarbAbstractItemView( QAbstractItemView ):
	def __init__(self, parent=None):
		super(MarbAbstractItemView, self).__init__( parent )

	def setScrollBarValues(self):
		None 
		
	
	def isIndexHidden(self, index):
		return False

	def resizeEvent(self, ev ):
		self.setScrollBarValues()
		return QAbstractItemView.resizeEvent(self, ev )


	def moveCursor(self, cursorAction, modifiers):
		if self.model() == None:
			return QModelIndex()
		return self.model().index(0,0)


	def itemRect(self, index):
		return QRectF()
	
	def itemRegion(self, index ):
		return QRegion( self.itemRect(index) )


	def horizontalOffset(self):
		return self.horizontalScrollBar().value()
	
	
	def verticalOffset(self):
		return self.verticalScrollBar().value()
		
			
	def indexAt(self, point):
		if self.model() == None:
			return QModelIndex()
		for index in self._itemPos.keys():
				r = self.itemRect(index)
				if r.contains( point ):
						return index
		return QModelIndex()

	
	def visualRect(self, index):
		r = self.itemRect(index)
		return r
	
	
	def visualRegionForSelection(self, selection):
		'''
		By default, we repaint the whole viewport."
		'''
		return QRegion( QRect(0, 0, self.width(), self.height() ) )


	def setSelection( self, rect, command ):
		contentsRect = rect.translated(
						self.horizontalScrollBar().value(),
						self.verticalScrollBar().value()).normalized()
		rows = self.model().rowCount( self.rootIndex() )
		columns = self.model().columnCount( self.rootIndex() )
		indexes = []

		for row in range( rows ):
			for col in range( columns ):
				index = self.model().index(row, col, self.rootIndex())
				region = self.itemRegion(index)
				if region.intersects(contentsRect):
					indexes.append(index)
		
		if len( indexes ) > 0:
			firstRow = indexes[0].row()
			lastRow = indexes[0].row()
			firstColumn = indexes[0].column()
			lastColumn = indexes[0].column()

			for i in range ( len( indexes ) ):
				firstRow = min(firstRow, indexes[i].row());
				lastRow = max(lastRow, indexes[i].row());
				firstColumn = min(firstColumn, indexes[i].column());
				lastColumn = max(lastColumn, indexes[i].column());
			
			selection = QItemSelection(
			self.model().index(firstRow, firstColumn, self.rootIndex()),
			self.model().index(lastRow, lastColumn, self.rootIndex()));
			self.selectionModel().select(selection, command);
		else:
			self.selectionModel().clear()
		
		self.viewport().repaint();
	
	def scrollTo( self, index, hint ):
		pass
	
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
