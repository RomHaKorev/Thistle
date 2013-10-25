from Marb.Global import Color, Render
from PySide.QtGui import QAbstractItemView, QPen, QRegion, QColor, QFontMetrics, QPainter, QPixmap, QPainterPath
from PySide.QtCore import QRect, QPointF, Qt, QModelIndex, QLineF
#from ..MarbAbstractItemView import MarbAbstractItemView
from .Chart import Chart
class PieChart( Chart ):
	class ColorConf:
		Normal = 0
		Lighter = 1
		Darker = 2
		Translucent = 4


	def __init__(self, parent=None):
		''' Constructor. Constructs an instance with the given [parent].
		parent: QWidget
		'''
		super(PieChart, self).__init__( parent )
		self.setEditTriggers( QAbstractItemView.NoEditTriggers )
		self._Ring = False
		self._Splitted = False
		self._StartAngle = 0.0
		self._Legend = ""
		self._Render = Render.Plain
		self._chartRect = QRect()
		self._valuesRect = QRect()
		self._ellipseFactor = 0.80


	def configureColor(self, painter, base, flag):
		''' Sets the pen and brush colors for the painter according to the bas color and the PieChart.ColorConf flags.
		'''
		painter.setPen( QPen( base, 3 ) )
		c = base
		if flag == PieChart.ColorConf.Darker:
			c = base.darker( 110 )
		elif flag == PieChart.ColorConf.Lighter:
			c = base.lighter( 120 )
		if self._Render == Render.Plain:
			painter.setPen( QPen( base.darker( 110 ), 2 ) )
		if self._Render == Render.Translucent:
				c.setAlpha( 180 )
		if self._Render == Render.Wireframe:
				c.setAlpha( 10 )
		painter.setBrush( c )


	def itemPart( self, angle, delta, splitted = False ):
		''' Returns a QPainterPath representing the pie part for the given angle and delta.
		'''
		part = QPainterPath()
		part.moveTo( self._valuesRect.center() )
		part.arcTo( self._valuesRect, -angle, -delta )
		if splitted == True:
			(x, y) = self.splittedOffset(angle, delta, True)
			part.translate( x, y )
		part.closeSubpath()
		if self._Ring == True:
			p = QPainterPath()
			p.addEllipse( self._valuesRect.center(), self._valuesRect.width() * 0.2, self._valuesRect.height() * 0.2 )
			part = part.subtracted( p )
		return part


	def itemPath( self, index ):
		''' Returns a QPainterPath representing the pie part for the given index.
		'''
		path = QPainterPath()
		if index.column() != 0:
			return path
		angle = self._StartAngle
		for r in range( 0, index.row() ):
			idx = self.model().index( r, 0 )
			v = abs( self.model().data( idx ) )
			delta = 360.0 * v/self._Total
			angle += delta
		v = abs( self.model().data( index ) )
		delta = 360.0 * v/self._Total
		if index in self.selectionModel().selectedIndexes():
			path = self.itemPart( angle, delta, True )
		else :
			path = self.itemPart( angle, delta )
		return path


	def paintChart( self, painter ):
		rows = self.model().rowCount()
		self.updateValues()
		painter.save()
		painter.setRenderHint( QPainter.Antialiasing )
		angle = self._StartAngle
		self._leftLabels = []
		self._rightLabels = []
		selectedIndexes = self.selectionModel().selectedIndexes()
		for i in range( 0, rows ):
			index = self.model().index( i, 0 )
			# Detect if model contains a specific color. Otherwise, use a predefined color.
			color = QColor( self.model().data( index, Qt.DecorationRole ) )
			if not color.isValid():
				color = Color.colorAt( i )
			value = 0
			try:
				value = float(self.model().data( index ))
			except:
				value = 0
			v = abs( value )
			delta = 360.0 * v/self._Total
			centerAngle = angle + delta/2.0
			if centerAngle < 90: # Right side
				self._rightLabels.append( (i, centerAngle, color) )
			elif centerAngle > 270: # Right side
				self._rightLabels.insert( 0, (i, centerAngle, color) )
			else: # Left side
				self._leftLabels.append( (i, centerAngle, color) )
			# A part can be splitted if self._Splitted is True or if self._Splitted is False and the value is selected in the model.
			isSelected = False
			if selectedIndexes != []:
				for idx in selectedIndexes:
					if idx == index:
						isSelected = True
						break
			if self._Splitted == False:
				if isSelected == False:
					self.paintPart( painter, angle, delta, color )
				else:
					self.paintPartSplitted( painter, angle, delta, color )
			else:
				if selectedIndexes != [] and isSelected == False:
					c = QColor( color )
					c.setAlpha( c.alpha() * 0.5 )
					self.paintPartSplitted( painter, angle, delta, c )
				else:
					self.paintPartSplitted( painter, angle, delta, color )
			angle += delta
		painter.drawText( 10, self._chartRect.bottomLeft().y(),
						self.width() - 20, self.height() - self._valuesRect.height(), Qt.AlignHCenter | Qt.TextWordWrap, self._Legend )
		painter.restore()


	def paintEvent( self, event ) :
		if ( self.model() == 0 ) :
			return None
		painter = QPainter( self.viewport() )
		self.paintChart( painter )


	def paintLabels(self, painter):
		''' Paints label for each part.
		'''
		# Paint labels
		metrics = QFontMetrics( self.font() )
		ell = QPainterPath()
		if self._Splitted == False:
			ell.addEllipse( self._valuesRect )
		else:
			ell.addEllipse( self._chartRect )
		l = []
		j = 1
		self._leftLabels.reverse()
		while j < len(self._leftLabels):
			(i, centerAngle, color) = self._leftLabels[j]
			prevCenterAngle = self._leftLabels[j - 1][1]
			y1 = ell.pointAtPercent( centerAngle/360.0 ).y()
			y2 = ell.pointAtPercent( prevCenterAngle/360.0 ).y()
			d = abs(y2 - y1)
			y = 0
			if d < (metrics.height() + 2):
				y = y1 + metrics.height() - d + 20
			else:
				y = y1
			l.append( (i, centerAngle, y, color) )
			j += 1
		for (i, centerAngle, y, color) in l:
			textPos = QPointF(0,0)
			p1 = QPointF(0,0)
			text = str(self.model().headerData( i, Qt.Vertical ))
			p = ell.pointAtPercent( centerAngle/360.0 )
			textPos = QPointF( self._leftLabelRect.x(), y )
			p1 = QPointF(textPos.x(), p.y() )
			r = QRect( textPos.x(), textPos.y() - metrics.height(), self._leftLabelRect.width(), metrics.height() )
			painter.drawText( r, text )
			pen = painter.pen()
			painter.setPen( color )
			painter.drawLine( p1, p )
			painter.setPen( pen )


	def paintPart( self, painter, angle, delta, color ):
		''' Paints the part defined by angle and delta with the given color.
		'''
		part = self.itemPart( angle, delta )
		painter.save()
		painter.setClipPath( part ) # To avoid the "borders superposition"
		self.configureColor(painter, color, PieChart.ColorConf.Lighter)
		painter.drawPath( part )
		painter.restore()


	def paintPartSplitted( self, painter, angle, delta, color ):
		''' Does the same work than paintPart but with an offset.
		Used to paint parts when the flag splitted is set.
		'''
		part = self.itemPart( angle, delta, True )
		painter.save()
		self.configureColor(painter, color, PieChart.ColorConf.Lighter)
		painter.drawPath( part )
		painter.restore()


	def save( self, filename ):
		pix = QPixmap( self.size() )
		pix.fill( Qt.white )
		painter = QPainter ( pix )
		self.paintChart( painter )
		return pix.save( filename )

	def scrollTo( self, index, hint ):
		pass


	def setLegend( self, legend ) :
		self._Legend = legend


	def setRing( self, ring ) :
		'''Defines the pie chart as a ring (paints a 'hole' at the center).
		'''
		self._Ring = ring


	def setSplitted( self, splitted ) :
		''' sets the flag self._Splitted to splitted.
		If splitted is True, 
		'''
		self._Splitted = splitted


	def splittedOffset( self, angle, delta, splitted ):
 		if splitted == False:
 			return ( 0, 0 )
 		line = QLineF( QPointF(0,0), QPointF((self._valuesRect.width()/2), 0) )
 		line.setAngle( -angle - delta/2 )
 		p1 = line.p2()
 		line.setLength( (line.length() * self._chartRect.width() / self._valuesRect.width()) )
 		p2 = line.p2()
 		return ( (p2.x() - p1.x()), (p2.y() - p1.y()) * 0.80 )


	def _updateRects(self):
		if self.model() == None:
			return None
		self.defineRects()
		self._titleRect.moveTo( self._valuesRect.bottomLeft() )
		self._titleRect.translate( (self._valuesRect.width() - self._titleRect.width())/2, 20 )


	def updateValues( self ):
		metrics = QFontMetrics( self.font() )
		maxLabelWidth = 0
		self._Total = 0
		self._updateRects()
		for i in range( 0, self.model().rowCount() ):
			index = self.model().index( i, 0 )
			value = 0
			try:
				value = float(self.model().data( index ))
			except:
				value = 0
			self._Total += abs( value )
			text = str(self.model().headerData( i, Qt.Vertical ))
			maxLabelWidth = max( metrics.width( text ), maxLabelWidth )
		w = self._chartRect.width()
		h = self._chartRect.height()
		if w < h:
			self._chartRect.setHeight( w )
			self._chartRect.translate( 0, (h- w)/2 )
		else:
			self._chartRect.setWidth( h )
			self._chartRect.translate( (w - h)/2, 0 )
		self._titleRect.moveTo( self._titleRect.x(), self._chartRect.bottom() + 10 )
		w = self._chartRect.width() * 0.80
		h = self._chartRect.height() * 0.80
		dw = self._chartRect.width() - w
		dh = self._chartRect.height() - h
		self._valuesRect = QRect( self._chartRect.x() + dw/2.0, self._chartRect.y() + dh/2.0, w, h )


	def visualRect( self, index ):
		# To force to repaint the whole chart. Not only the area of the part
		return QRect( 0, 0, self.width(), self.height() )


	def visualRegionForSelection( self, selection ):
		region = QRegion()
		region += QRect( 0, 0, self.width(), self.height() )
		return region


