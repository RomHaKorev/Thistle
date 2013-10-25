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
			''' Sets the background brush.
			*brush: QBrush
			'''
			self._brush = brush
		
		def setBorder(self, pen):
			''' Sets the QPen used to paint item border.
			*pen: QPen
			'''
			self._border = pen
		
		def setFont(self, font):
			''' Sets the font used to write item text.
			*font: QFont
			'''
			self._font = font
		
		def setTextColor(self, color):
			'''Sets the text color to [color].
			*color: QColor
			'''
			self._textColor = color
		
		def setDisplayText(self, display = True):
			''' If [display] is True, the item text will be painted. Otherwise, the text will not be visible.
			*display: boolean
			'''
			self._displayText = display
			
		def setShape(self, shape):
			'''Sets the item shape to shape.
			*shape: Global.Shape
			'''
			self._shape = shape
					 
		def background(self):
			''' Returns the QBrush used to paint the item background.
			'''
			return self._brush
		
		def border(self):
			''' Returns the QPen used to paint the item border.
			'''
			return self._border
		
		def font(self):
			''' Returns the QFont used to paint the item text.
			'''
			return self._font
				
		def textColor(self):
			''' Returns the QColor used to paint the item text.
			'''
			return self._textColor
		
		def displayText(self):
			''' Returns True if the text will be displayed. Otherwise returns False
			'''
			return self._displayText
		
		def shape(self):
			''' Returns the Global.Shape of the items.
			'''
			return self._shape
				
				
class MarbItemDelegate(QStyledItemDelegate):
	def __init__(self, parent=None):
		super(MarbItemDelegate, self).__init__(parent)
		self._textVisible = True
		self._itemStyle = MarbItemStyle()
	
	def setItemStyle(self, style):
		'''Sets the item style to [style]
		*style: MarbItemStyle
		'''
		self._itemStyle = style
	
	def itemStyle(self):
		'''Return the current MarbItemStyle
		'''
		return self._itemStyle
					
	def _createDiamond(self, rect):
		'''Returns a QPolygon containing a diamond shape bounded by the given rect.
		*rect: Qrect
		'''
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