from ..Global import Shape, Color

from PySide.QtGui import QPen, QPolygon, QStyleOptionViewItem, QBrush, QColor, QStyle, QPainterPath, QPainter, QFontMetrics, QFont, QRadialGradient
from PySide.QtCore import QPoint, Qt, QRect, QSize

from .IndicatorDelegate import IndicatorDelegate

class RoundedIndicatorDelegate( IndicatorDelegate ):
	'''PointDelegate class provides display facilities for the Linear Chart.
	When displaying data from a linear Chart series configured with the flag Marb.Type, the values are drawn with the relative delegate.
	'''
	def __init__( self, parent = None ):
		''' Constructor. Constructs an instance with the given 'parent'.
		'''
		super(RoundedIndicatorDelegate, self).__init__( parent )


# Qt::FontRole	6	The font used for items rendered with the default delegate. (QFont)
# Qt::TextAlignmentRole	7	The alignment of the text for items rendered with the default delegate. (Qt::AlignmentFlag)
# Qt::BackgroundRole	8	The background brush used for items rendered with the default delegate. (QBrush)
# Qt::BackgroundColorRole	8	This role is obsolete. Use BackgroundRole instead.
# Qt::ForegroundRole	9	The foreground brush (text color, typically) used for items rendered with the default delegate. (QBrush)
# Qt::TextColorRole	9	This role is obsolete. Use ForegroundRole instead.
# Qt::CheckStateRole	10	This role is used to obtain the checked state of an item. (Qt::CheckState)
# Qt::InitialSortOrderRole	14	This role is used to obtain the initial sort order of a header view section. (Qt::SortOrder). This role was introduced in Qt 4.8.

	def _paintDisabled(self, painter, option, index ):
		'''Paints the index value when the index is disabled, according to the columnStyle shape.
		An index is disabled if the selection is not empty and doesn't contains the index.
		'''
		raise( NotImplementedError, "Should be implemented" )

	def _getRectInOut( self, s1, rect ):
		if s1 == None:
			w = min( rect.width(), rect.height() )
			s1 = QSize( w * 0.9, w * 0.9 )
		else:
			s1 = QSize( s1.width() * 0.9, s1.height() * 0.9 )
		s2 = QSize( s1.width() * 0.8, s1.height() * 0.8 )
		
		rectOut = QRect( -( s1.width()) /2, -(s1.height())/2, s1.width(), s1.height() )
		rectIn = QRect( -( s2.width()) /2, -(s2.height())/2, s2.width(), s2.height() )
		rectOut.translate( rect.center() )
		rectIn.translate( rect.center() )
		return ( rectIn, rectOut )


	def _paintEnabled(self, painter, option, index ):
		'''Paints the index value when the index is enabled, according to the columnStyle shape.
		An index is enabled if the selection is empty or contains the index.
		'''
		painter.save()

		( font, fontValue, bgColor, fgColor, labelColor, valueColor ) = self._getParameters( index )
		value = index.data()
		label = index.data( Qt.UserRole )
		( valueStr, value ) = self._formatValue( value )
		
		( rectIn, rectOut ) = self._getRectInOut( index.data( Qt.SizeHintRole ), option.rect )
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
		painter.drawText( rectLabel, Qt.AlignCenter, label )

		painter.setPen( QPen( valueColor ) )
		painter.setFont( fontValue )
		painter.drawText( rectValue, Qt.AlignCenter, valueStr )

		painter.setPen( Qt.NoPen )
		painter.setBrush( bgColor )
		painter.drawPath( path )
		painter.setBrush( fgColor )
		painter.setClipPath( clipPath )
		painter.drawPath( path )

		c = QColor( 20, 20, 20, 50 )
		gradient = QRadialGradient( rectOut.center(), rectOut.width()/2. + 5 )
		gradient.setColorAt( 0, QColor( Qt.transparent ) )
		gradient.setColorAt( 0.9, QColor( Qt.transparent ) )
		gradient.setColorAt( 1, c )
		painter.setBrush( gradient )
		painter.setClipPath( path )
		painter.drawEllipse( rectOut )
		gradient = QRadialGradient( rectOut.center(), rectOut.width()/2.0 )
		gradient.setColorAt( 0, c )
		gradient.setColorAt( 0.80 * (rectIn.width()/rectOut.width()), c )
		gradient.setColorAt( 0.85, QColor( Qt.transparent ) )
		painter.setBrush( gradient )
		#painter.drawEllipse( rectOut )
		painter.drawEllipse( rectOut )
		painter.restore()


