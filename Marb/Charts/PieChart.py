from ..Global import Color, Shape, Type

from PySide.QtGui import QAbstractItemView, QPen, QBrush, \
QRegion, QColor, QFontMetrics, QPainter, QPixmap, QPainterPath
from PySide.QtCore import QRect, QPoint, Qt, QModelIndex, QSize

class PieChart( QAbstractItemView ):
	def __init__(self, parent=None):
		super(PieChart, self).__init__( parent )
		self.setEditTriggers( QAbstractItemView.NoEditTriggers )
		self._Ring = False
		self._Splitted = False
		self._StartAngle = 0
		self._Legend = ""
	

	def visualRect( self, index ):
		# To force to repaint the whole chart. Not only the area of the part
		return QRect( 0, 0, self.width(), self.height() )
	

	def visualRegionForSelection( self, selection ):
		region = QRegion()
		region += QRect( 0, 0, width(), height() )
		return region
	

	def scrollTo( self, index, hint ):
		pass


	def indexAt( self, point ):
		x = point.x() + horizontalOffset()
		y = point.y() + verticalOffset()

		for r in range( 0, self.model().rowCount() ) :
			index = self.model().index( r, 0 )
			path = itemPath( index )
			if ( path.contains( QPointF( x, y ) ) ) :
				return index
		
		return Qself.modelIndex()
	

	def moveCursor( self, cursorAction, modifiers ) :
		return QModelIndex()


	def horizontalOffset( self ):
		return 0
	

	def verticalOffset( self ):
		return 0
	

	def isIndexHidden( self, index ):
		return False
	

	def updateChart( self ):
		w = self.width() - 40
		h = self.height() - 40
		if self._Legend == "":
			metrics = QFontMetrics( self.font() )
			r = metrics.boundingRect( 10, 0, self.width() - 20, self.height(), Qt.AlignHCenter | Qt.TextWordWrap, self._Legend )
			h -= 10 + r.height()
		
		if w < h:
			self._Rect = QRect( 20, 20 + (h-w)/2, w , w )
		else :
			self._Rect = QRect( 20 + (w-h)/2, 20, h , h )
		
		self._Total = 0
		
		for i in range( 0, self.model().rowCount() ):
			index = self.model().index( i, 0 )
			self._Total += abs( self.model().data( index ) )
		
	

	def paintEvent( self, event ) :
		if ( self.model() == 0 ) :
			return None
		
		painter = QPainter( self.viewport() )
		self.paintChart( painter )

	


	def paintChart( self, painter ):
		rows = self.model().rowCount()

		self.updateChart()

		painter.save()
		painter.setRenderHint( QPainter.Antialiasing )
		
		angle = self._StartAngle
		for i in range( 0, rows ):
			index = self.model().index( i, 0 )
			color = QColor( self.model().data( index, Qt.DecorationRole ) )
			if not color.isValid():
				color = Color.colorAt( i )
				
			v = abs( self.model().data( index ) )
			delta = 360.0 * v/self._Total

			if self._Splitted == False:
				self.paintPart( painter, angle, delta, color )
			else:
				self.paintPartSplitted( painter, angle, delta, color )
			
			angle += delta
		
		painter.drawText( 10, self._Rect.bottomLeft().y() + 10,
						self.width() - 20, self.height() - self._Rect.height(), Qt.AlignHCenter | Qt.TextWordWrap, self._Legend )
		painter.restore()
	

	def paintPart( self, painter, angle, delta, color ):		
		part = self.itemPart( angle, delta )

		painter.save()
		painter.setClipPath( part ) # To adef the "borders superposition"
		painter.setPen( QPen( color, 4 ) )
		color = color.lighter( 120 )
		painter.setBrush( color )
		painter.drawPath( part )

		painter.restore()
	

	def paintPartSplitted( self, painter, angle, delta, color ):
		part = self.itemPart( angle, delta, True )

		painter.save()
		painter.setPen( QPen(color, 2) )
		color = color.lighter( 120 )
		painter.setBrush( color )
		painter.drawPath( part )
		painter.restore()
	

	def itemPath( self, index ):
		path = QPainterPath()
		angle = self._StartAngle
		for r in range( 0, index.row() ):
			id = self.model().index( r, 0 )
			v = abs( self.model().data( id ) )
			delta = 360.0 * v/self._Total
			angle += delta
		
		v = abs( self.model().data( index ) )
		delta = 360.0 * v/self._Total

		if ( selectionself.model().selectedIndexes().contains( index )) :
			path = self.itemPart( angle, delta, True )
		else :
			path = self.itemPart( angle, delta )
		
		return path
	

	def itemPart( self, angle, delta, splitted = False ):
		part = QPainterPath()
		part.moveTo( self._Rect.center() )
		part.arcTo( self._Rect, -angle, -delta )

		if splitted == True:
			part.closeSubpath()
			p = part.pointAtPercent( 0.5 )
			line( p, self._Rect.center() )
			line.setLength( line.length() + 10 )
			p = line.p2()
			r = self._Rect.translated( self._Rect.center().x() - p.x(), self._Rect.center().y() - p.y() )
			part = QPainterPath()
			part.moveTo( r.center() )
			part.arcTo( r, -angle, -delta )	

		part.closeSubpath()

		if self._Ring == True:
			p = QPainterPath()
			p.addEllipse( self._Rect.center(), self._Rect.width() * 0.3, self._Rect.height() * 0.3 )
			part = part.subtracted( p )
		
		return part
		

	def setRing( self, ring ) :
		self._Ring = ring
	

	def setSplitted( self, splitted ) :
		self._Splitted = splitted
	

	def setLegend( self, legend ) :
		self._Legend = legend
	

	def save( self, filename ):
		pix = QPixmap( size() )
		pix.fill( Qt.white )
		painter = QPainter ( pix )
		self.paintChart( painter )
		return pix.save( filename )
	
