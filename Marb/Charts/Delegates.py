from ..Global import Shape

from PySide.QtGui import QPen, QStyledItemDelegate, QPolygon, QStyleOptionViewItem, QRadialGradient, QLinearGradient, QBrush, QColor, QStyle
from PySide.QtCore import QPoint, Qt, QRect, QSize

class ChartDelegate( QStyledItemDelegate ):
	'''PointDelegate class provides display facilities for the Linear Chart.
	When displaying data from a linear Chart series configured with the flag Marb.Type, the values are drawn with the relative delegate.
	'''
	def __init__( self, parent ):
		''' Constructor. Constructs an instance with the given 'parent'.
		'''
		super(ChartDelegate, self).__init__( parent )


	def paint( self, painter, option, index ):
		if ( option.state == QStyle.State_Off ):
			self._paintDisabled(painter, option, index)
		else:
			self._paintEnabled(painter, option, index)


	def _paintEnabled(self, painter, option, index ):
		'''Paints the index value when the index is enabled, according to the columnStyle shape.
		An index is enabled if the selection is empty or contains the index.
		'''
		raise( NotImplementedError, "Should be implemented" )


	def _paintDisabled(self, painter, option, index ):
		'''Paints the index value when the index is disabled, according to the columnStyle shape.
		An index is disabled if the selection is not empty and doesn't contains the index.
		'''
		raise( NotImplementedError, "Should be implemented" )



class PointDelegate( ChartDelegate ):
	def _createDiamond(self, rect):
		''' Returns a polygon representing a diamond. The dimensions and the position are fixed by the QRect rect.
		The method is called when you use the flag Marb.Shape.Diamond
		'''
		poly = QPolygon()
		poly.append( rect.topLeft() + QPoint(rect.width()/2, 0) )
		poly.append( rect.topRight() + QPoint(0, rect.height()/2) )
		poly.append(rect.bottomLeft() + QPoint( rect.width()/2, 0 ))
		poly.append(rect.topLeft() + QPoint( 0, rect.height()/2 ))
		return poly


	def _paintEnabled( self, painter, option, index ):
		chartItemStyle = self.parent().columnStyle( index.column() )

		painter.save()
		painter.setBrush( chartItemStyle.brush() )
		painter.setPen( chartItemStyle.pen() )
		r = option.rect

		if option.state == QStyle.State_Selected:
			r = QRect( -(r.width() * 1.5)/2.0, -(r.height() * 1.5 )/2.0,
				   r.width() * 1.5, r.height() * 1.5 ).translated( r.center() )

		if chartItemStyle.shape() == Shape.Ellipse:
			painter.drawEllipse( r )
		elif chartItemStyle.shape() == Shape.RoundedRect:
			painter.drawRoundedRect( r, 5, 5 )
		elif chartItemStyle.shape() == Shape.Diamond:
			painter.drawPolygon( self._createDiamond( r ) )
		elif chartItemStyle.shape() == Shape.Triangle:
			poly = QPolygon()
			poly.append( r.topLeft() + QPoint( r.width()/2, 0 ) )
			poly.append( r.bottomLeft() )
			poly.append( r.bottomRight() )
			painter.drawPolygon( poly )
		elif chartItemStyle.shape() == Shape.ReversedTriangle:
			poly = QPolygon()
			poly.append( r.bottomLeft() + QPoint( r.width()/2, 0 ) )
			poly.append( r.topLeft() )
			poly.append( r.topRight() )
			painter.drawPolygon( poly )
		else:
			painter.drawRect( r )
		painter.restore()


	def _paintDisabled(self, painter, option, index ):
		chartItemStyle = self.parent().columnStyle( index.column() )
		painter.save()
		r = option.rect
		r2 = QRect( r.topLeft() - QPoint( 5, 5 ), r.bottomRight() + QPoint( 5, 5 ) )
		painter.setPen( Qt.NoPen )
		gradient = QRadialGradient( r2.center(), r2.width() / 2.0, r2.center() )
		c = QColor( Qt.gray )
		c.setAlpha( 150 )
		gradient.setColorAt( 0, c )
		gradient.setColorAt( 1, Qt.transparent )
		gradient.setSpread( QRadialGradient.ReflectSpread )
		painter.setBrush( QBrush( gradient ) )
		painter.drawEllipse( r2 )
		painter.restore()




class BarDelegate( ChartDelegate ):
	'''BarDelegate class provides display facilities for the Linear Chart.
	
	When displaying data from a linear Chart series configured with the flag Type.Bar, the values are drawn with this delegate.
	'''
	def __init__( self, parent ):
		super(BarDelegate, self).__init__( parent )


	def _paintEnabled( self, painter, option, index ):
		chartItemStyle = self.parent().columnStyle( index.column() )
		r = option.rect

		painter.save()

		painter.setBrush( chartItemStyle.brush() )
		painter.setPen( Qt.NoPen )
		painter.drawRect( r )

		polygon = QPolygon()
		if option.decorationPosition == QStyleOptionViewItem.Top: # Positive value
			polygon.append( r.bottomLeft() )
			polygon.append( r.topLeft() )
			polygon.append( r.topRight() )
			polygon.append( r.bottomRight() )
		else:
			polygon.append( r.topLeft() )
			polygon.append( r.bottomLeft() )
			polygon.append( r.bottomRight() )
			polygon.append( r.topRight() )
		
		painter.setBrush( Qt.NoBrush )
		pen = QPen(chartItemStyle.pen())
		pen.setCapStyle( Qt.FlatCap )
		painter.setPen( pen )		
		painter.drawPolyline( polygon )
		if option.state == QStyle.State_Selected:
			painter.setClipRect( r )
			pen.setWidth( pen.width() * 5 )
			painter.setPen( pen )
			painter.drawPolyline( polygon )
		
		painter.restore()

	def _paintDisabled( self, painter, option, index ):
		chartItemStyle = self.parent().columnStyle( index.column() )
		r = option.rect.normalized()
		
		painter.save()
		
		painter.setPen( Qt.NoPen )
		gradient = QLinearGradient( r.topLeft() + QPoint( r.width()/2, 0 ), r.topLeft()  )
		gradient.setSpread( QLinearGradient.ReflectSpread )
		c = QColor( Qt.gray )
		c.setAlpha( 100 )
		gradient.setColorAt( 0, c )
		gradient.setColorAt( 0.4, c )
		gradient.setColorAt( 1, Qt.transparent )
		painter.setBrush( QBrush( gradient ) )
		painter.setClipRect( r )
		painter.drawRect( r )

		painter.restore()


