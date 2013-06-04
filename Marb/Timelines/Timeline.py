from ..Global import Color, Shape, Type

from PySide.QtGui import QAbstractItemView, QPen, QBrush, \
QRegion, QColor, QFontMetrics, QPainter, QPixmap, QPainterPath, QTransform, QStyleOptionViewItem
from PySide.QtCore import QRect, QPoint, Qt, QModelIndex, QSize

class TimelineStyle:
	def __init__(self):
		self._pen = QPen( QBrush(Color.Green), 3 )
	
	def pen(self):
		return QPen( self._pen )
	
	def setPen( self, pen ):
		self._pen = pen
		
class Timeline(QAbstractItemView):
	'''The Chart class provides an abstract base for the chart viewes.
	
	The Chart class defines the standard interface for every chart views in Marb. It is not supposed to be instantiated directly but should be subclassed.  
	'''
	def __init__(self, parent=None):
		super(Timeline, self).__init__( parent )
		self._itemRect = QRect()
		self._path = QPainterPath()
		self._referencePath = QPainterPath()
		self._timelineStyle = TimelineStyle()
	
	##
	## Qt methods
	##
	def itemRect(self, index):
		'''Returns the QRect in which the index value is displayed on the view.
		'''
		return QRect( -10, -10, 20, 20 )
	
		
	def resizeEvent(self, ev ):
		'''Overloaded method.
		'''
		QAbstractItemView.resizeEvent(self, ev )
		self.updatePath()
	
	
	def moveCursor(self, cursorAction, modifiers):
		'''Overloaded method.
		'''
		return QModelIndex()
	
	
	def horizontalOffset(self):
		return self.horizontalScrollBar().value()
	
	
	def verticalOffset(self):
		return self.verticalScrollBar().value()


	def indexAt(self, point):
		'''Overloaded method.
		'''
		return QModelIndex() # BUG IN PySide
		p = point - QPoint( self.horizontalOffset(), self.verticalOffset() )
		for index in self._itemPos.keys():
			r = self.itemRect(index)
			if r.contains( p ):
				None
		return index

	
	def visualRect(self, index):
		'''Overloaded method.
		'''
		r = self.itemRect(index)
		return r
	
	
	def visualRegionForSelection(self, selection):
		'''Overloaded method.
		'''
		ranges = selection.count()
		region = QRegion()
		for i in range(0, ranges):
			ran = self.selection.at(i)
			for row in range( ran.top(), ran.bottom() ):
				index = self.model().index( row, 0, self.rootIndex() )
				region += self.visualRect( index )
	

	def rows(self, index):
		'''Overloaded method.
		'''
		return self.model().rowCount( self.model().parent(index))
	
	
	def setModel(self, *args, **kwargs):
		'''Overloaded method.
		'''
		QAbstractItemView.setModel(self, *args, **kwargs)
		self.updatePath()
	
	
	def dataChanged(self, top_left, bottomRight ):
		'''Overloaded method.
		'''
		QAbstractItemView.dataChanged(self, top_left, bottomRight)
		self.updatePath()
		self.update( self.model().index(0,0) )
	

	def rowsAboutToBeRemoved(self, parent, start, end):
		'''Overloaded method.
		'''
		QAbstractItemView.rowsAboutToBeRemoved(self, parent, start, end)
		self.updatePath()
		self.viewport().update()
		self.setScrollBarValues()
	
	
	def rowsInserted(self, parent, start, end):
		'''Overloaded method.
		'''
		QAbstractItemView.rowsInserted(self, parent, start, end)
		self.process()
		self.viewport().update()
		self.setScrollBarValues()

	def setPath(self, path):
		self._referencePath = path
	
	def updatePath(self):
		factorW= 1
		try:
			factorW = float(self.width() - 20) / self._referencePath.boundingRect().width()
		except ZeroDivisionError as e:
			factorW= -1
		try:
			factorH = float(self.height() - 20) / self._referencePath.boundingRect().height()
		except ZeroDivisionError as e:
			factorH= -1
				
		if factorW == -1 or factorH == -1:
			factor = max(1, factorW, factorH)
		elif factorW > 1 or factorH > 1:
			factor = min( factorW, factorH )
		else:
			factor = 1
		
		transform = QTransform()
		transform.scale( factor, factor )
		self._path = transform.map( self._referencePath )
		
		print factorW, factorH, factor
		
		centerView = QPoint( self.width()/2, self.height()/2 )
		centerPath = self._path.boundingRect().center()
		self._path.translate( centerView.x() - centerPath.x(), centerView.y() - centerPath.y() )

	def paintEvent( self, event ) :
		if self.model() == None:
			return None
		
		painter = QPainter( self.viewport() )
		painter.setRenderHints(QPainter.Antialiasing | QPainter.TextAntialiasing )
		painter.setClipRect( event.rect() )
		self.paintTimeline( painter )


	def paintTimeline( self, painter ):
		painter.save()
		painter.setPen(self._timelineStyle.pen())
		painter.drawPath( self._path )
		for r in range( self.model().rowCount() ):
			offset = self._path.pointAtPercent( float(r + 0.5)/float(self.model().rowCount()) )
			index = self.model().index( r, 0 )
			option = QStyleOptionViewItem()
			option.rect = self.itemRect( index ).translated( offset.x(), offset.y() )
			self.itemDelegate().paint( painter, option, index )
		painter.restore()

	
	def save(self, filename, size = None ):
		if size == None:
			size = self.size()
		pixmap = QPixmap( size )
		pixmap.fill( Qt.white )
		painter = QPainter( pixmap )
		self.process()
		self.paintTimeline( painter )
		painter.end()
		return pixmap.save( filename )
	
