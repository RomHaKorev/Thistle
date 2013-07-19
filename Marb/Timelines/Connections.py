from PySide.QtGui import QColor, QStyledItemDelegate, QPen, QBrush
from PySide.QtCore import Qt, QLineF

class TimelineConnection():
	def __init__(self):
		self._pen = QPen()
		self._brush = QBrush()
		
		
	def setPen(self, pen):
		if not isinstance( pen, QPen ):
			raise "pen must be a QPen"
		self._pen = pen
	
	def setBrush(self, brush):
		if not isinstance( brush, QBrush ):
			raise "pen must be a QBrush"
		self._brush = brush
		
	def paint(self, painter, anchorTimeline, anchorItem):
		raise NotImplementedError
	
	
	
class simpleTLConnection( TimelineConnection ):
	def paint(self, painter, anchor, itemRect):
		painter.save()
		painter.setPen( self._pen )
		painter.setBrush( self._brush )
		p2 = itemRect.center()
		line = QLineF(anchor, p2)
		painter.drawLine( line )
		painter.restore()