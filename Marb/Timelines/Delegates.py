from PySide.QtGui import QColor, QStyledItemDelegate, QPen
from PySide.QtCore import Qt, QRect
from PySide import QtGui
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
	def __init__(self, parent = None ):
		super(SimpleTimelineItemDelegate, self).__init__( parent )
	
#	def paint(self, painter, option, index ):
#		r = option.rect
#		text = index.data( Qt.DisplayRole )
#		
#		#painter.drawRect( option.rect )
#		painter.save()
#		painter.setPen( QColor(20, 20, 20) )
#		r1 = QRect( r.x() + 2, r.y() + 2, r.width() - 4, r.height() - 4 )
#		painter.drawText( r1, Qt.TextWordWrap, str(text) )
#		
#		painter.restore()

	def paint(self, painter, option, index):
		options = QtGui.QStyleOptionViewItemV4(option)
		self.initStyleOption(options,index)
		
		style = QtGui.QApplication.style() if options.widget is None else options.widget.style()
		
		doc = QtGui.QTextDocument()
		doc.setHtml(options.text)
		doc.setPageSize( option.rect.size() )
		options.text = ""
		style.drawControl(QtGui.QStyle.CE_ItemViewItem, options, painter);
		
		ctx = QtGui.QAbstractTextDocumentLayout.PaintContext()
		
		textRect = style.subElementRect(QtGui.QStyle.SE_ItemViewItemText, options)
		painter.save()
		painter.translate(textRect.topLeft())
		doc.documentLayout().draw(painter, ctx)
		painter.restore()
		painter.save()
		painter.setBrush( Qt.NoBrush )
		painter.setPen( Qt.red )
		painter.drawRect( option.rect )
		painter.restore()
	
	def sizeHint(self, option, index):
		options = QtGui.QStyleOptionViewItemV4(option)
		self.initStyleOption(options,index)
		
		doc = QtGui.QTextDocument()
		doc.setHtml(options.text)
		doc.setTextWidth(options.rect.width())
		return QtCore.QSize(doc.idealWidth(), doc.size().height())
	
	
#	def paintConnection( self, painter, option, index ):
#		if index.row() == 0 or index.row() == index.model().rowCount() - 1:
#			painter.drawEllipse( option.rect.center(), 10, 10 )
#		else:
#			painter.drawEllipse( option.rect.center(), 5, 5 )