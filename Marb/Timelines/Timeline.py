from ..Global import Color, Shape, Type

from PySide.QtGui import QAbstractItemView, QPen, QBrush, \
QRegion, QColor, QFontMetrics, QPainter, QPixmap, QPainterPath, QTransform, QStyleOptionViewItem
from PySide.QtCore import QRect, QPoint, Qt, QModelIndex, QSize


from .Delegates import SimpleTimelineItemDelegate, TimelineItemDelegate

#	path = QPainterPath()
#	path.moveTo(0, 0);
#	path.cubicTo(0, 0,  50, 60,  50, 200);
# 	path.moveTo(150, 50);
#	path.cubicTo(60, 50,  50, 100,  50, 200);

class TimelineStyle:
	def __init__(self):
		self._pen = QPen( QBrush(Color.Green), 3 )
		self._brush = QBrush( Color.LightGreen )
	def brush(self):
		return QBrush( self._brush )
	
	def pen(self):
		return QPen( self._pen )
	
	def setBrush(self, brush):
		self._brush = brush
	
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
		self._referenceItemPath =  QPainterPath()
		self._itemPath =  QPainterPath()
		self._timelineStyle = TimelineStyle()
		self._rect = QRect( -25, -25, 50, 50 )
		
		self.setItemDelegate( SimpleTimelineItemDelegate( self ) )
	
	##
	## Qt methods
	##
	def itemRect(self, index):
		'''Returns the QRect in which the index value is displayed on the view.
		'''
		
		path = self._path
		if self._itemPath != QPainterPath():
			path = self._itemPath
		
		if not index.isValid():
			return QRect()
		offset = path.pointAtPercent( float(index.row() + 0.5)/float(self.model().rowCount()) )
	
		return self._rect.translated( offset.x(), offset.y() )
	
		
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
		'''Set the path to display on the view
		This path will be used by default if itemPath is not setted
		'''
		self._referencePath = path
		
		
	def setItemPath(self, path):
		'''Set the path that items must follow. This path is used to calculate the item position.
		The path will be not displayed in the view.
		'''
		self._referenceItemPath = path

	
	def updatePath(self):
		
		r1 = self._referenceItemPath.boundingRect()
		r2 = self._referencePath.boundingRect()
		tl = QPoint( min(r1.topLeft().x(), r2.topLeft().x()),
					min(r1.topLeft().y(), r2.topLeft().y()) )
		br = QPoint( max(r1.bottomRight().x(), r2.bottomRight().x()),
					max(r1.bottomRight().y(), r2.bottomRight().y()) )
		rect = QRect( tl, br )
		
		factorW= 1
		factorH = 1
		try:
			factorW = float(self.width() - 20 - self._rect.width()) / rect.width()
		except ZeroDivisionError as e:
			factorW= -1
		try:
			factorH = float(self.height() - 20 - self._rect.height()) / rect.height()
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
		self._itemPath = transform.map( self._referenceItemPath )
		
		r1 = self._itemPath.boundingRect()
		r2 = self._path.boundingRect()
		tl = QPoint( min(r1.topLeft().x(), r2.topLeft().x()),
					min(r1.topLeft().y(), r2.topLeft().y()) )
		br = QPoint( max(r1.bottomRight().x(), r2.bottomRight().x()),
					max(r1.bottomRight().y(), r2.bottomRight().y()) )
		rect = QRect( tl, br )
		centerView = QPoint( self.width()/2, self.height()/2 )
		centerPath = rect.center()
			
		self._path.translate( centerView.x() - centerPath.x(), centerView.y() - centerPath.y() )
		self._itemPath.translate( centerView.x() - centerPath.x(), centerView.y() - centerPath.y() )
		 
		#print self._itemPath.boundingRect(), self._path.boundingRect()

	def paintEvent( self, event ) :
		if self.model() == None:
			return None
		
		painter = QPainter( self.viewport() )
		painter.setRenderHints(QPainter.Antialiasing | QPainter.TextAntialiasing )
		painter.setClipRect( event.rect() )
		self.paintTimeline( painter )


	def paintTimeline( self, painter ):
		painter.save()
		painter.drawPath( self._itemPath )
		painter.setPen(self._timelineStyle.pen())
		painter.setBrush( self._timelineStyle.brush() )
		painter.drawPath( self._path )

		for r in range( self.model().rowCount() ):
			index = self.model().index( r, 0 )
			option = QStyleOptionViewItem()
			option.rect = self.itemRect( index )
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
	
