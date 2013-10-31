from ..Global import Shape, Color

from PySide.QtGui import QPen, QStyledItemDelegate, QPolygon, QStyleOptionViewItem, QBrush, QColor, QStyle, QPainterPath, QPainter, QFontMetrics, QFont
from PySide.QtCore import QPoint, Qt, QRect, QSize

class RoundedPercentDelegate( QStyledItemDelegate ):
	'''PointDelegate class provides display facilities for the Linear Chart.
	When displaying data from a linear Chart series configured with the flag Marb.Type, the values are drawn with the relative delegate.
	'''
	def __init__( self, parent = None ):
		''' Constructor. Constructs an instance with the given 'parent'.
		'''
		super(RoundedPercentDelegate, self).__init__( parent )


	def paint( self, painter, option, index ):
		painter.setRenderHint( QPainter.Antialiasing )
		self._paintEnabled(painter, option, index)
		# if ( option.state == option.state | QStyle.State_Enabled ):
		# 	self._paintEnabled(painter, option, index)
		# else:
		# 	self._paintDisabled(painter, option, index)

# Qt::FontRole	6	The font used for items rendered with the default delegate. (QFont)
# Qt::TextAlignmentRole	7	The alignment of the text for items rendered with the default delegate. (Qt::AlignmentFlag)
# Qt::BackgroundRole	8	The background brush used for items rendered with the default delegate. (QBrush)
# Qt::BackgroundColorRole	8	This role is obsolete. Use BackgroundRole instead.
# Qt::ForegroundRole	9	The foreground brush (text color, typically) used for items rendered with the default delegate. (QBrush)
# Qt::TextColorRole	9	This role is obsolete. Use ForegroundRole instead.
# Qt::CheckStateRole	10	This role is used to obtain the checked state of an item. (Qt::CheckState)
# Qt::InitialSortOrderRole	14	This role is used to obtain the initial sort order of a header view section. (Qt::SortOrder). This role was introduced in Qt 4.8.

	def _getParameters( self, index ):

		fontValue = index.data( Qt.TextColorRole )
		if fontValue == None:
			fontValue = QFont()
			fontValue.setPixelSize( 24 )
		font = QFont( fontValue )
		font.setPixelSize( font.pixelSize() / 2 )
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

	def _paintEnabled(self, painter, option, index ):
		'''Paints the index value when the index is enabled, according to the columnStyle shape.
		An index is enabled if the selection is empty or contains the index.
		'''
		painter.save()

		( font, fontValue, bgColor, fgColor, labelColor, valueColor ) = self._getParameters( index )
		value = index.data()
		label = index.data( Qt.UserRole )
		valueStr = str( value * 100 ) + ' %'
		
		s1 = index.data( Qt.SizeHintRole )
		if s1 == None:
			w = min( option.rect.width(), option.rect.height() )
			s1 = QSize( w * 0.9, w * 0.9 )
		else:
			s1 = QSize( s1.width() * 0.9, s1.height() * 0.9 )
		s2 = QSize( s1.width() * 0.8, s1.height() * 0.8 )
		
		rectOut = QRect( -( s1.width()) /2, -(s1.height())/2, s1.width(), s1.height() )
		rectIn = QRect( -( s2.width()) /2, -(s2.height())/2, s2.width(), s2.height() )
		rectOut.translate( option.rect.center() )
		rectIn.translate( option.rect.center() )

		metrics = QFontMetrics( font )
		rectLabel = metrics.boundingRect( rectIn, Qt.AlignCenter, label ).normalized()

		metrics = QFontMetrics( fontValue )
		rectValue = metrics.boundingRect( rectIn, Qt.AlignCenter, valueStr ).normalized()

		h = rectLabel.height() + rectValue.height() + 8
		rectLabel.translate( 0, -h/2 )
		rectValue.translate( 0, 4 )

		path = QPainterPath()
		path.addEllipse( rectOut )
		
		clipPath = QPainterPath()
		x = rectOut.x() + rectOut.width() / 2
		y = rectOut.y() + rectOut.height() / 2
		clipPath.moveTo( x, y )
		clipPath.arcTo( rectOut.x(), rectOut.y(),
						rectOut.width(), rectOut.height(),
						0, - 360.0 * value )

		path.addEllipse( rectIn )

		painter.setFont( font )
		painter.setPen( labelColor )
		painter.drawText( rectLabel, Qt.AlignCenter, label );

		painter.setPen( QPen( valueColor ) )
		painter.setFont( fontValue )
		painter.drawText( rectValue, Qt.AlignCenter, valueStr );

		painter.setPen( Qt.NoPen )
		painter.setBrush( bgColor )
		painter.drawPath( path )
		painter.setBrush( fgColor )
		painter.setClipPath( clipPath )
		painter.drawPath( path )

		painter.restore()


	def _paintDisabled(self, painter, option, index ):
		'''Paints the index value when the index is disabled, according to the columnStyle shape.
		An index is disabled if the selection is not empty and doesn't contains the index.
		'''
		raise( NotImplementedError, "Should be implemented" )