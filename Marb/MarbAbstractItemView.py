from PySide.QtGui import QAbstractItemView, QTableView, QItemSelection, QPainterPath, QRubberBand, QItemSelectionModel, QRegion
from PySide.QtCore import QModelIndex, QPoint, QRect, QSize

class MarbAbstractItemView( QAbstractItemView ):
	def __init__(self, parent=None):
		super(MarbAbstractItemView, self).__init__( parent )
		self.rubberBand = QRubberBand(QRubberBand.Rectangle, self.viewport())


	def _calculateOrderedColumn( self ):
		return range( self.model().columnCount() )


	def dataChanged(self, top_left, bottomRight ):
		QAbstractItemView.dataChanged(self, top_left, bottomRight)
		self.viewport().update()


	def horizontalOffset(self):
		return self.horizontalScrollBar().value()


	def indexAt(self, point):
		if self.model() == None:
			return QModelIndex()
		for index in self._itemPos.keys():
				r = self.itemRect(index)
				if r.contains( point ):
						return index
		return QModelIndex()


	def isIndexHidden(self, index):
		return False


	def itemRect(self, index):
		return QRect()


	def itemPath(self, index ):
		path = QPainterPath()
		path.addRect( self.itemRect( index ) )
		return path


	def mouseMoveEvent( self, event ):
		self.rubberBand.setGeometry(QRect(self._rubberBandOrigin, event.pos()).normalized())
		super(MarbAbstractItemView, self).mouseMoveEvent( event )


	def mousePressEvent( self, event ):
		super(MarbAbstractItemView, self).mousePressEvent( event )
		self._rubberBandOrigin = event.pos()
		self.rubberBand.setGeometry(QRect(event.pos(), QSize()))
		self.rubberBand.show()


	def mouseReleaseEvent(self, event):
		super(MarbAbstractItemView, self).mouseReleaseEvent( event )
		if abs(self.rubberBand.width()) > 10 and abs(self.rubberBand.height()) > 10:
			self.setSelection( self.rubberBand.geometry(), QItemSelectionModel.Select )
		self.viewport().update()
		self.rubberBand.hide();


	def moveCursor(self, cursorAction, modifiers):
		if self.model() == None:
			return QModelIndex()
		idx = self.currentIndex()

		if cursorAction == QAbstractItemView.MoveLeft or cursorAction == QAbstractItemView.MoveDown:
			if idx.row() > 0:
				idx = self.model().index(idx.row() - 1, idx.column(), self.rootIndex() )
		elif cursorAction == QAbstractItemView.MoveRight or cursorAction == QAbstractItemView.MoveUp:
			if idx.row() < self.rows(idx) - 1:
				idx = self.model().index(idx.row() + 1, idx.column(), self.rootIndex())
			else:
				idx = self.model().index( self.rows(idx) - 1, idx.column(), self.rootIndex() )
		return idx


	def paintEvent(self, event):
		raise( NotImplementedError )


	def resizeEvent(self, ev ):
		self.setScrollBarValues()
		return QAbstractItemView.resizeEvent(self, ev )


	def rows(self, index):
		return self.model().rowCount( self.model().parent(index))


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


	def save(self, filename ):
		raise( NotImplementedError )


	def scrollTo( self, index, hint ):
		pass


	def setModel(self, *args, **kwargs):
		QAbstractItemView.setModel(self, *args, **kwargs)
		self.updateValues()


	def setScrollBarValues(self):
		None 


	def setSelection( self, rect, command ):
		contentsRect = rect.translated(
						self.horizontalScrollBar().value(),
						self.verticalScrollBar().value()).normalized()
		rows = self.model().rowCount( self.rootIndex() )
		columns = self._calculateOrderedColumn()
		count = 0
		contentsPath = QPainterPath()
		contentsPath.addRect( contentsRect )
		for row in range( rows ):
			for col in columns:
				index = self.model().index(row, col, self.rootIndex())
				path = self.itemPath(index)
				if not path.intersected(contentsPath).isEmpty():
					count += 1
					self.selectionModel().select(index, command)

		if count == 0:
			self.selectionModel().clear()
		self.viewport().update()


	def updateValues( self ):
		raise( NotImplementedError )


	def verticalOffset(self):
		return self.verticalScrollBar().value()


	def visualRect(self, index):
		r = self.itemRect(index)
		return r


	def visualRegionForSelection(self, selection):
		'''
		By default, we repaint the whole viewport."
		'''
		return QRegion( QRect(0, 0, self.width(), self.height() ) )


