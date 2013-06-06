from PySide.QtGui import QColor, QStyledItemDelegate
from PySide.QtCore import Qt

#class TimelineDelegate( QStyledItemDelegate ):
#	def __init__(self, parent ):
#		super(TimelineDelegate, self).__init__( parent )
#	
#	def paint(self, painter, option, index ):
#		path = QPainterPath()
#		r = option.rect
#		path.moveTo(r.topLeft())		
#		path.cubicTo(0, 0,  50, 60,  50, 200)
#		path.moveTo(150, 50)
#		path.cubicTo(60, 50,  50, 100,  50, 200)

class TimelineItemDelegate( QStyledItemDelegate ):
	def __init__(self, parent ):
		super(TimelineItemDelegate, self).__init__( parent )
	
	def paint(self, painter, option, index ):
		raise NotImplementedError


class SimpleTimelineItemDelegate( TimelineItemDelegate ):
	def __init__(self, parent ):
		super(SimpleTimelineItemDelegate, self).__init__( parent )
	
	def paint(self, painter, option, index ):
		r = option.rect
		text = index.data( Qt.DisplayRole )
				
		painter.save()
		painter.setPen( QColor(20, 20, 20) )
		painter.setBrush( Qt.NoBrush )
		painter.drawText( r, Qt.AlignHCenter | Qt.TextWordWrap, str(text) )
		painter.restore()
	
	
#	def paintConnection( self, painter, option, index ):
#		if index.row() == 0 or index.row() == index.model().rowCount() - 1:
#			painter.drawEllipse( option.rect.center(), 10, 10 )
#		else:
#			painter.drawEllipse( option.rect.center(), 5, 5 )