from ..Global import Shape

from PySide.QtGui import QPen, QStyledItemDelegate, QPolygon, QStyleOptionViewItem, QRadialGradient, QLinearGradient, QBrush, QColor, QStyle
from PySide.QtCore import QPoint, Qt, QRect


class PointDelegate( QStyledItemDelegate ):
	'''PointDelegate class provides display facilities for the Linear Chart.
	
	When displaying data from a linear Chart series configured with the flag Marb.Type.Point, the values are drawn with this delegate.
	'''
	def __init__( self, parent ):
		super(PointDelegate, self).__init__( parent )
		

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
		
	
	def paint( self, painter, option, index ):
		'''Paints the data of the index relative to the shape set in the ChartStyle corresponding to the column of index.
		'''
		if ( option.state == QStyle.State_Off ):
			self._paintDisabled(painter, option, index)
		else:
			self._paintEnabled(painter, option, index)
	
	def _paintEnabled( self, painter, option, index ):
		chartItemStyle = self.parent().columnStyle( index.column() )

		painter.save()
		painter.setBrush( chartItemStyle.brush() )
		painter.setPen( chartItemStyle.pen() )
		r = option.rect

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

class BarDelegate( QStyledItemDelegate ):
	'''BarDelegate class provides display facilities for the Linear Chart.
	
	When displaying data from a linear Chart series configured with the flag Type.Bar, the values are drawn with this delegate.
	'''
	def __init__( self, parent ):
		super(BarDelegate, self).__init__( parent )


	def paint( self, painter, option, index ):
		'''Paints the data of the index relative to the shape set in the ChartStyle corresponding to the column of index.
		'''
		chartItemStyle = self.parent().columnStyle( index.column() )
		r = option.rect

		painter.save()

		painter.setBrush( chartItemStyle.brush() )
		painter.setPen( Qt.NoPen )
		painter.drawRect( r )

		polygon = QPolygon()
		if option.decorationPosition == QStyleOptionViewItem.Top: # Negative value
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
		
		painter.restore()

	def paintDisabled( self, painter, option, index ):
		'''Paints the data of the index relative to the shape set in the ChartStyle corresponding to the column of index.
		'''
		chartItemStyle = self.parent().columnStyle( index.column() )
		r = option.rect

		painter.save()

		painter.setPen( Qt.NoPen )
		gradient = QLinearGradient( r.topLeft() + QPoint( 5, 0 ), r.topLeft()  )
		c = QColor( Qt.gray )
		c.setAlpha( 150 )
		gradient.setColorAt( 0, c )
		gradient.setColorAt( 1, Qt.transparent )
		painter.setBrush( QBrush( gradient ) )
		
		r1 = QRect( r.topLeft() + QPoint( 0, 5 ),
				r.bottomLeft() + QPoint( 10, -5 ) )
		
		painter.drawRect( r1 )
		
		gradient = QLinearGradient( r.topRight() - QPoint( 5, 0 ), r.topRight()  )
		c = QColor( Qt.gray )
		c.setAlpha( 150 )
		gradient.setColorAt( 0, c )
		gradient.setColorAt( 1, Qt.transparent )
		painter.setBrush( QBrush( gradient ) )
		
		r2 = QRect( r.topRight() + QPoint( -10, 5 ),
				r.bottomRight() + QPoint( 0, 5 ) )
		painter.drawRect( r2 )
		
		c = QColor( Qt.gray )
		c.setAlpha( 150 )
		painter.setBrush( QBrush( c ) )
		painter.drawRect( QRect( r1.topRight() + QPoint(1,0), r2.bottomLeft() - QPoint(1,0) ) )
		
		painter.restore()


