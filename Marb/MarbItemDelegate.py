from .Global import Color, Shape

from PySide.QtGui import QPen, QStyledItemDelegate, QBrush, QFont, QPolygon, \
QLineEdit, QRegion, QColor, QPainter, QStyleOptionViewItem
from PySide.QtCore import QSize, QRect, QPoint, QPointF, Qt, QModelIndex

class MarbItemStyle:
		def __init__(self):
				self._brush = QBrush( Color.Blue )
				self._border = QPen( QColor( Color.DarkBlue ), 1.5 )
				self._font = QFont()
				self._textColor = QColor( Qt.white )
				self._displayText = True
				self._shape = Shape.Rectangle
				self.itemOffset = QPointF(0,0)
		
		def setBackground(self, brush):
				self._brush = brush
		
		
		def setBorder(self, pen):
				self._border = pen
		
		
		def setFont(self, font):
				self._font = font
		
				
		def setTextColor(self, color):
				self._textColor = color
		
		
		def setDisplayText(self, b = True):
				self._displayText = b
		
		
		def setShape(self, shape):
				self._shape = shape
		
					 
		def background(self):
				return self._brush
		
		
		def border(self):
				return self._border
		
		
		def font(self):
				return self._font
		
				
		def textColor(self):
				return self._textColor
		
		
		def displayText(self):
				return self._displayText
		
		
		def shape(self):
				return self._shape
				
				
class MarbItemDelegate(QStyledItemDelegate):
		def __init__(self, parent=None):
				super(MarbItemDelegate, self).__init__(parent)
				self._textVisible = True
				self._itemStyle = MarbItemStyle()
		
		
		def setItemStyle(self, style):
				self._itemStyle = style
		
		def itemStyle(self):
			return self._itemStyle
		
						
		def _createDiamond(self, rect):
				poly = QPolygon()
				poly.append( rect.top_left() + QPoint(rect.width()/2, 0) )
				poly.append( rect.topRight() + QPoint(0, rect.height()/2) )
				poly.append(rect.bottom_left() + QPoint( rect.width()/2, 0 ))
				poly.append(rect.top_left() + QPoint( 0, rect.height()/2 ))
				return poly
		
		
		def createEditor(self, parent, option, index):
				editor = QLineEdit( parent )
				editor.setGeometry( option.rect )
				return editor
		
		
		def setEditorData(self, editor, index):
				if isinstance( editor , QLineEdit):
						editor.setText( str( index.model().data( index ) ) )
		
				
		def setModelData(self, editor, model, index):
				if isinstance( editor, QLineEdit):
						model.setData( index, editor.text(), Qt.DisplayRole )
		
		
		def paint(self, painter, option, index):
				painter.save()
				painter.setBrush( self._itemStyle.background() )
				painter.setPen( self._itemStyle.border() )
				r = option.rect
				
				if self._itemStyle.shape() == Shape.Ellipse:
						painter.drawEllipse( r )
				elif self._itemStyle.shape() == Shape.RoundedRect:
						painter.drawRoundedRect( r, 5, 5 )
				elif self._itemStyle.shape() == Shape.Diamond:
						painter.drawPolygon( self.createDiamond( r ) )
				elif self._itemStyle.shape() == Shape.Triangle:
						poly = QPolygon()
						poly.append( r.topLeft() + QPoint( r.width()/2, 0 ) )
						poly.append( r.bottomLeft() )
						poly.append( r.bottomRight() )
						painter.drawPolygon( poly )
				elif self._itemStyle.shape() == Shape.ReversedTriangle:
						poly = QPolygon()
						poly.append( r.bottomLeft() + QPoint( r.width()/2, 0 ) )
						poly.append( r.topLeft() )
						poly.append( r.topRight() )
						painter.drawPolygon( poly )
				else:
						painter.drawRect( r )
				
				if self._itemStyle.displayText() == True:
						painter.setPen( QPen( self._itemStyle.textColor(), 1 ) )
						flags = Qt.AlignCenter
						if self._itemStyle.shape() == Shape.Triangle:
								flags = Qt.AlignBottom | Qt.AlignHCenter
						elif self._itemStyle.shape() == Shape.ReversedTriangle:
								flags = Qt.AlignTop | Qt.AlignHCenter
						
						painter.drawText( r, flags, str( index.model().data(index) ) )
				
				painter.restore()
			
						
		def updateEditorGeometry(self, editor, option, index):
				None