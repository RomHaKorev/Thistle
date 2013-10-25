from Marb.Global import Color, Render

from PySide.QtGui import QColor, QPainter, QPainterPath, QPolygonF, QFontMetrics
from PySide.QtCore import QRect, QPointF, Qt

from ..Charts import PieChart

class PieChart3D( PieChart ):
	def __init__( self, parent = None ):
		super(PieChart3D, self).__init__( parent )
		self._Splitted = False
		self._Angles = []
		
	
	def updateValues( self ) :
		metrics = QFontMetrics( self.font() )
		maxLabelWidth = 0
		self._Total = 0
		
		self._updateRects()
		
		w = float( self._chartRect.width() )
		h = float( self._chartRect.height() )
		
		if h >= (w * 0.8):
			h = w * 0.8
		else:
			w = h / 0.8
		
		self._chartRect.setX( self._chartRect.x() + ( self._chartRect.width() - w )/2.0 )
		self._chartRect.setY( self._chartRect.y() + ( self._chartRect.height() - h )/2.0 )
		self._chartRect.setWidth( w )
		self._chartRect.setHeight( h )
		
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

		self._titleRect.moveTo( self._titleRect.x(), self._chartRect.bottom() + 10 )
		w = self._chartRect.width() * 0.80
		h = self._chartRect.height() * 0.80
		dw = self._chartRect.width() - w
		dh = self._chartRect.height() - h
		self._valuesRect = QRect( self._chartRect.x() + dw/2.0, self._chartRect.y() + dh/2.0, w, h )

		self._Height = self._chartRect.height() * 0.20
		#self._chartRect.setHeight( self._chartRect.height() * 0.80 )
		self._valuesRect.setHeight( self._valuesRect.height() * 0.80 )

		self._Angles = []
		angle = 0

		for i in range( self.model().rowCount() ):
			self._Angles.append(angle)
			index = self.model().index( i, 0 )
			value = 0
			try:
				value = float(self.model().data( index ))
			except:
				value = 0
			v = abs( value )
			delta = 360.0 * v/self._Total
			self._Angles.append(delta)
			angle += delta
		
		self._Angles.append(360.0)


	def paintChart( self, painter) :
		painter = QPainter( self.viewport() )
		painter.save()
		painter.setRenderHint( QPainter.Antialiasing )
		
		self.updateValues()

	
		#if self._Render != Render.Plain :
		self.paintExternal( painter, True )
		self.paintSides( painter )
		self.paintExternal( painter, False )
#		else:
#			self.paintExternal( painter, False )
		
		i = 0
		while ( i < len( self._Angles ) - 2 ):		
			index = self.model().index( i/2, 0 )
			color = QColor( self.model().data( index, Qt.DecorationRole ) )
			if not color.isValid():
				color = Color.colorAt( int(i/2) )
			isSelected = index in self.selectionModel().selectedIndexes()
			if self._Splitted == False:
				if isSelected == False:
					self.paintPart( painter, self._Angles[i], self._Angles[i + 1], color )
				else:
					self.paintPartSplitted( painter, self._Angles[i], self._Angles[i + 1], color )
			else:
				if self.selectionModel().selectedIndexes() != [] and isSelected == False:
					c = QColor( color )
					c.setAlpha( c.alpha() * 0.5 )
					self.paintPartSplitted( painter, self._Angles[i], self._Angles[i + 1], c )
				else:
					self.paintPartSplitted( painter, self._Angles[i], self._Angles[i + 1], color )
			i+=2
		
		painter.restore()
		font = self.font()
		font.setItalic( True )
		painter.setFont( font )
		painter.drawText( self._titleRect, Qt.AlignHCenter | Qt.AlignTop | Qt.TextWordWrap, self._title )
#		painter.setPen( Qt.red )
#		painter.drawRect( self._titleRect )
#		painter.setPen( Qt.blue )
#		painter.drawRect( self._chartRect )
#		painter.setPen( Qt.green )
#		painter.drawRect( self._valuesRect )

	def paintExternal( self, painter, top ) :
		i = 0
		while i < (len( self._Angles ) - 2 ):
			index = self.model().index( i/2, 0 )
			color = QColor( self.model().data( index, Qt.DecorationRole ) )
			if not color.isValid():
				color = Color.colorAt( int(i/2) )
			
			painter.save()
			a1 = self._Angles[i]
			delta = self._Angles[i + 1]
			a2 = a1 + delta
			
			if top == False:
				if a1 <= 180 and a2 > 180:
					painter.restore()
					i += 2
					continue
				
				if a1 > 180 or a2 > 180:
					painter.restore()
					i += 2
					continue
			else :
				if a1 < 180 and a2 < 180:
					painter.restore()
					i += 2
					continue
			
			isSelected = self._Splitted or (index in self.selectionModel().selectedIndexes())
			path = self.itemExternalPart( a1, delta, isSelected )
	
			self.configureColor( painter, color, PieChart.ColorConf.Darker )
			
			if self._Render != Render.Plain:
				if a1 < 180 and a2 > 180:
					self.paintLeft( painter, a1, delta );
				painter.drawPath( path )
			else:
				painter.drawPath( path )
				if a1 < 180 and a2 > 180:
					self.paintLeft( painter, a1, delta, isSelected );
			painter.restore()
			i += 2
	
	
	
	def paintSides( self, painter ) :
		rightBottom = []
		rightTop = []
		leftBottom = []
		leftTop = []
		
		i = 0
		while ( i < len( self._Angles) - 2 ):
			index = self.model().index( i/2, 0 )
			color = QColor( self.model().data( index, Qt.DecorationRole ) )
			if not color.isValid():
				color = Color.colorAt( int(i/2) )
			
			isSelected = self._Splitted or (index in self.selectionModel().selectedIndexes())
			offset = self.splittedOffset( self._Angles[i], self._Angles[i + 1], isSelected )
	
			path = self.side( self._Angles[i], offset, isSelected )
	
			if self._Angles[i] <= 90:
				rightBottom.append( (path, color) )
			elif self._Angles[i] <= 180:
				leftBottom.insert(0, (path, color) )
			elif self._Angles[i] <= 270:
				leftTop.insert(0, (path, color) )
			else:
				rightTop.append( (path, color) )
			
			path = self.side( self._Angles[i] + self._Angles[i+1], offset, isSelected )
	
			if self._Angles[i] <= 90:
				rightBottom.append( (path, color) )
			elif self._Angles[i] <= 180:
				leftBottom.insert(0, (path, color) )
			elif self._Angles[i] <= 270:
				leftTop.insert(0, (path, color) )
			else:
				rightTop.append( (path, color) )
			i += 2
		

		l = leftTop + rightTop + leftBottom + rightBottom
		for pair in l:
			self.configureColor( painter, pair[1], PieChart.ColorConf.Lighter )
			painter.drawPath( pair[0] )

	
	def side( self, angle, centerOffset, splitted ):
		ell = QPainterPath()
	
		r = QRect(self._valuesRect)
		
		if (splitted == True ) :
			r.translate( centerOffset[0], centerOffset[1] )
		
		ell.addEllipse( r )
		p1 = ell.pointAtPercent( round(angle / 360.0, 4) )
		center = r.center()
		offset = QPointF( 0, self._Height )
	
		outside = QPainterPath()
		outside.setFillRule( Qt.WindingFill )
	
	
		sides = QPolygonF()
		sides.append( center )
		sides.append( p1 )
		sides.append( p1 + offset )
		sides.append( center + offset.toPoint() )
		sides.append( center )
	
		outside.moveTo( center )
		outside.addPolygon( sides )
	
		return outside
	
	
	def itemSidesPath( self, index ):
		outside = QPainterPath()
	
		angle = self._StartAngle
		for r in range( index.row() ):
			idx = self.model().index( r, 0 )
			v = abs( float(self.model().data( idx )) )
			delta = 360.0 * v/self._Total
			angle += delta
		
	
		v = abs( float(self.model().data( index )) )
		delta = 360.0 * v/self._Total
	
		if self.selectionModel().selectedIndexes().contains( index ):
			outside = self.itemExternalPart( angle, delta, True )
		else:
			outside = self.itemExternalPart( angle, delta )
		
		return outside
	
	
	def itemExternalPart( self, angle, delta, splitted ):
		ell = QPainterPath()
		ell.addEllipse( self._valuesRect )
		p1 = ell.pointAtPercent( angle / 360.0 )
		offset = QPointF( 0, self._Height )
	
		outside = QPainterPath()
		outside.setFillRule( Qt.WindingFill )
	
		outside.moveTo( p1 )
		outside.arcTo( self._valuesRect, -angle, -delta )
		outside.lineTo( outside.currentPosition() + offset )
		outside.arcTo( self._valuesRect.translated( 0, self._Height ), -angle - delta, delta )
		outside.lineTo( p1 )
	
		if splitted == True :
			(x, y) = self.splittedOffset( angle, delta, True )
			outside.translate( x, y )
		
		return outside
	
	
	def setRender( self, r ):
		self._Render = r
	
		
	def paintLeft( self, painter, angle, delta, splitted ):
		if self._Render == Render.Wireframe:
			return None
		painter.save()
		#self.configureColor( painter, color, 3 );
		width = painter.pen().width()/2.0
		painter.setPen( Qt.NoPen )
		path = QPainterPath()
		ellipse1 = QPainterPath()
		ellipse2 = QPainterPath()
		ellipse1.addEllipse( self._valuesRect )
		ellipse2.addEllipse( self._valuesRect.translated( 0, self._Height ) )
		
		(x, y) = self.splittedOffset( angle, delta, splitted )
		ellipse1.translate( x, y )
		ellipse2.translate( x, y )
		
		path.moveTo( ellipse1.pointAtPercent( 0.5 ) + QPointF( -width, 0 ) )
		path.lineTo( ellipse2.pointAtPercent( 0.5 ) + QPointF( -width, 0 ) )
		path.arcTo( self._valuesRect.translated( 0, self._Height ), 180, -90 )
		path.moveTo( ellipse1.pointAtPercent( 0.5 ) )
		path.arcTo( self._valuesRect, 180, 90 )
		path = path.subtracted( ellipse1 )
		path = path.subtracted( ellipse2 )
		
		painter.fillPath( path, painter.brush().color() )
		painter.restore()
		
	