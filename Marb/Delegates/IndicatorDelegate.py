from ..Global import Shape, Color

from PySide.QtGui import QStyledItemDelegate, QBrush, QColor, QFont, QPainter
from PySide.QtCore import Qt

class IndicatorDelegate( QStyledItemDelegate ):
	def __init__( self, parent = None ):
		''' Constructor. Constructs an instance with the given 'parent'.
		'''
		super(IndicatorDelegate, self).__init__( parent )
		self._minimum = 0
		self._maximum = 100
		self._suffix = " %"


	def _getParameters( self, index ):

		fontValue = index.data( Qt.TextColorRole )
		if fontValue == None:
			fontValue = QFont()
			fontValue.setPixelSize( 12 )
		font = QFont( fontValue )
		#font.setPixelSize( font.pixelSize() / 2 )
		font.setBold( True )

		bgColor = index.data( Qt.BackgroundRole )
		if bgColor == None:
			bgColor = QColor( 0xDEDEDE )

		fgColor = index.data( Qt.ForegroundRole )
		if fgColor == None:
			fgColor = QColor( Color.LightBlue )

		labelColor = index.data( Qt.UserRole + 1 )
		if labelColor == None:
			labelColor = QColor( Color.Gray )

		valueColor = index.data( Qt.TextColorRole )
		if valueColor == None:
			valueColor = QColor( Color.DarkGreen )

		return ( font, fontValue, bgColor, fgColor, labelColor, valueColor )

	def _formatValue( self, value ):
		text = str( value ) + self._suffix
		if self._maximum == self._minimum:
			v = self._maximum
		else:
			v = ( float( value ) - self._minimum ) / ( self._maximum - self._minimum )
		return ( text, v )


	def paint( self, painter, option, index ):
		#super(IndicatorDelegate, self).paint( painter, option, index )
		painter.setRenderHint( QPainter.Antialiasing )
		self._paintEnabled(painter, option, index)
		# if ( option.state == option.state | QStyle.State_Enabled ):
		# 	self._paintEnabled(painter, option, index)
		# else:
		# 	self._paintDisabled(painter, option, index)


	def setMaximum( self, max ):
		self._maximum = max


	def setMinimum( self, min ):
		self._minimum = min


	def setSuffix( self, suffix ):
		self._suffix = " " + suffix.rstrip(" ")