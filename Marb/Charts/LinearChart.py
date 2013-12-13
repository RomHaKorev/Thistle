from ..Global import Color, Type
from .Chart import Chart, ChartStyle, Axis
from .Delegates import PointDelegate, BarDelegate
from PySide.QtGui import QPainter, QPen, QStyleOptionViewItem, QColor, QFontMetrics, QLinearGradient, QBrush, QStyle, QPainterPath
from PySide.QtCore import QSize, QRect, QPointF, QPoint, Qt, QLineF, QModelIndex


class StraightAxis(Axis):
	def __init__( self ):
		super( StraightAxis, self ).__init__()
		self.angle = 90.0
		self.angleAxis = -90.0
		self.xAxisLength = 0
		self.pen = QPen( QColor(Color.LightGray), 1.5 )
		self.xStep = 0
		self.valueCount = 1
		self.xAxis = QLineF()

	def paint( self, painter ):
		painter.save()
		painter.setPen( self.pen )
		line = QLineF( self.origin, self.origin + QPoint(0,10) )
		line.setAngle( self.angle )
		line.setLength( self.length )

		self.xAxis = QLineF( self.origin, self.origin + QPoint(0,10) )
		self.xAxis.setAngle( self.angle + self.angleAxis )
		self.xAxis.setLength( self.xAxisLength )

		painter.drawLine( line )
		self._paintXAxis( painter )
		c = painter.pen().color()
		c.setAlpha( 150 )
		painter.setPen( QPen( c , 1 ) )
		y = self.minBound
		while y <= self.maxBound:
			p1 = QPoint( self.origin.x(), self.valueToPx(y) )
			p2 = p1 + QPoint( self.xAxisLength, 0 )
			l = QLineF( p1, p2 )
			(intersectType, intersectionPoint) = l.intersect( self.xAxis )
			if intersectType == QLineF.BoundedIntersection:
				l.setP2( intersectionPoint )
			painter.drawLine( l )
			y += self.tickSize
		painter.restore()


	def _paintXAxis( self, painter ):
		painter.drawLine( self.xAxis )
		n = self.valueCount - 1
		i = 0
		for i in range( 1, n ):
			x = self.xAxis.pointAt( float(i)/float(n) )
			l = QLineF( x - QPoint(0, 3), x + QPoint(0, 3) )
			painter.drawLine( l )



class LinearChart(Chart):
	''' LinearChart class provides a view for QAbstractItemModel to display data in each column as line chart or bar chart.
	To each column in the model corresponds a serie of data.
	'''


	def __init__(self, parent=None):
		super(LinearChart, self).__init__( parent )
		#self._yAxis.origin = QPointF(20, 10)
		self._yAxis = StraightAxis()
		self._pointDelegate = PointDelegate( self )
		self._barDelegate = BarDelegate( self )
		self._minBottomMargin = 0
		self._verticalLabel = False
		self._dataStartonYAxis = False



	def _barStyleColumns(self):
		'''Returns the list of column with the type Bar. Used to calculate the width of each bar.
		See Marb.Global.Type and setColumnType(), columnType()
		'''
		bars = []
		for c in range( 0, self.model().columnCount() ):
			if self.columnType( c ) == Type.Bar:
				bars.append( c )
		return bars


	def _calculateOrderedColumn(self):
		'''Calculates in which order the view should paint the series of data.
		'''
		bars = []
		lines = []
		points = []
		for c in range(0, self.model().columnCount()):
			t = self.columnType(c)
			if t | Type.Bar == t:
				bars.append(c)
			elif t | Type.Line == t:
				lines.append(c)
			else:
				points.append( c )
		return bars + lines + points


	def columnType(self, column):
		'''Returns the type for the given column.
		See Marb.Global.Type and setColumnType()
		'''
		if column in self._style:
			return self._style[ column ].type()
		return ChartStyle().type()


	def itemRect(self, index ):
		'''Overloaded method.
		'''
		r = QRect()
		t = self.columnType( index.column() )
		value = index.data()
		if value == None:
			return QRect()
		y = self._yAxis.valueToPx(value)
		x = self._yAxis.origin.x() + index.row() * self._yAxis.xStep
		space = self._yAxis.xStep * 0.2
		orderedColumns = self._calculateOrderedColumn()
		if t == Type.Bar:
			bars = self._barStyleColumns()
			w = float(self._yAxis.xStep) / len(bars)
			x += w * orderedColumns.index( index.column() )
			tl = QPoint( x + space/2, y )             # top left
			br = QPoint( x + w, self._yAxis.origin.y() )    # bottom right
			r = QRect( tl, br )
			if value < 0:
				r.translate( 0, 1 )
			else:
				r.translate( 0, -2 )
		else:
			if self._dataStartonYAxis == False:
				r = QRect( -5, -5, 10 ,10 ).translated( x + self._yAxis.xStep/2, y ) 
			else:
				r = QRect( -5, -5, 10 ,10 ).translated( x, y ) 
		return r.normalized()


	def paintChart(self, painter):
		'''Overloaded method.
		'''
		painter.setRenderHints( QPainter.Antialiasing | QPainter.TextAntialiasing )
		self._yAxis.paint( painter )
		ordered = self._calculateOrderedColumn()
		for c in ordered:
			self._paintValues( painter, c )
		self._paintTextAxis(painter)
		self._paintLegend(painter)
		font = self.font()
		font.setItalic( True )
		painter.setFont( font )
		painter.drawText( self._titleRect, Qt.AlignHCenter | Qt.AlignTop | Qt.TextWordWrap, self._title )


	def _paintColumnLegend(self, painter, c, pos, metricsH):
		p1 = pos + QPoint( 10, - metricsH/2 )
		p2 = pos + QPoint( 40, - metricsH/2 )
		posText = pos + QPoint( 45, 0 )
		s = str(self.model().headerData( c, Qt.Horizontal ))
		style = self.columnStyle(c)
		t = style.type()
		painter.drawText( posText, s )
		painter.save()
		painter.setPen( style.pen() )
		if t | Type.Line == t:
			painter.drawLine( p1, p2 )
		if t == Type.Bar:
			option = QStyleOptionViewItem()
			j = 0
			for i in (15, 8, 17, 5 ):
				painter.setPen( Qt.NoPen )
				painter.setBrush( style.brush() )
				r = QRect( 10 + pos.x() + j * 8, pos.y() - i, 5, i )
				painter.drawRect( r )
				j += 1			
		elif t | Type.Point == t:
			option = QStyleOptionViewItem()
			option.rect = QRect( p1.x() + abs(p1.x() - p2.x())/2 - 5, p1.y() - 5, 10, 10 )
			self._pointDelegate.paint( painter, option, self.model().index(0,c) )
		painter.restore()


	def _paintTextAxis(self, painter):
		painter.save()
		metrics = QFontMetrics( self.font() )
		h = metrics.height()
		textPos = QPoint( h/2 , self._yAxis.origin.y() + 5 );
		x = self._yAxis.xStep + self._yAxis.origin.x()
		i = 0
		
		while (i < self.model().rowCount() ):
			p1 = QPoint( x, self._yAxis.origin.y() - 3 )
			s = str(self.model().headerData( i, Qt.Vertical ))
			painter.save()
			painter.setPen( QPen( QColor(Color.DarkGray), 1.5 ) )
			if self._verticalLabel == True:
				painter.rotate( -90 )
				if self._dataStartonYAxis == False:
					painter.translate( -textPos.y() - metrics.width( s ) - 3 , p1.x() - self._yAxis.xStep/2.0 )
				else:
					painter.translate( -textPos.y() - metrics.width( s ) - 3 , p1.x() - self._yAxis.xStep + h )
				painter.drawText( 0, 0, s )
			else:
				if self._dataStartonYAxis == False:
					painter.drawText( p1.x() - self._yAxis.xStep/2.0 - metrics.width( s )/2.0, textPos.y() + h, s )
				else:
					painter.drawText( p1.x() - self._yAxis.xStep, textPos.y() + h, s )
			painter.restore()
			i += 1
			x += self._yAxis.xStep
		y = self._yAxis.minBound
		while y <= self._yAxis.maxBound:
			p1 = QPoint( self._yAxis.origin.x(), self._yAxis.valueToPx(y) )
			s = str(round(y, self._yAxis.nbDigits))
			s = s.rstrip("0")
			s = s.rstrip(".")
			r = QRect( QPoint( 0, p1.y() - h/2 ), QSize( self._yAxis.origin.x() - 5 ,h) )
			painter.drawText( r, Qt.AlignRight, s )
			y += self._yAxis.tickSize
		painter.restore()


	def _paintValues(self, painter, column):
		t = self.columnType(column)
		delegate = None
		if t | Type.Point == t:
			delegate = self._pointDelegate
		elif t == Type.Bar:
			delegate = self._barDelegate
		rows = self.model().rowCount()
		painter.save()
		style = self.columnStyle( column )
		painter.setBrush( style.brush() )
		painter.setPen( style.pen() )
		isActive = True
		if self.selectionModel() != None:
			selectedIndexes = self.selectionModel().selectedIndexes()
			if len( selectedIndexes ) != 0:
				isActive = False
				for idx in selectedIndexes:
					if idx.column() == column:
						isActive = True
						break
		for r in range( rows ):
			index = self.model().index( r, column )
			option = QStyleOptionViewItem()
			try:
				value = float( index.data() )
			except:
				value = 0
			if value < 0:
				option.decorationPosition = QStyleOptionViewItem.Bottom
			else:
				option.decorationPosition = QStyleOptionViewItem.Top
			if isActive == False:
				option.state = QStyle.State_Off
			elif index in self.selectionModel().selectedIndexes():
				option.state = QStyle.State_Selected
			option.rect = self.itemRect( index )
			if t | Type.Line == t:
				if r < (rows - 1):
					if isActive == False:
						p1 = option.rect.center()
						p2 = self.itemRect( self.model().index( r + 1, column ) ).center()
						line = QLineF( p1, p2 )
						l = QLineF( line.pointAt( 0.5 ), line.p2() ).normalVector()
						l.setLength( 4 )
						gradient = QLinearGradient( l.p1(), l.p2() )
						c = QColor( Qt.darkGray )
						c.setAlpha( 50 )
						gradient.setColorAt( 0, c )
						gradient.setColorAt( 1, Qt.transparent )
						gradient.setSpread( QLinearGradient.ReflectSpread )
						painter.save()
						pen = QPen( QBrush( gradient ), 8 )
						painter.setPen( pen ) 
						painter.drawLine( p1, p2 )
						painter.restore()
					else:
						p1 = option.rect.center()
						p2 = self.itemRect( self.model().index( r + 1, column ) ).center()
						painter.drawLine( p1, p2 )
			if delegate != None:
				delegate.paint( painter, option, index )
		painter.restore()


	def process( self ):
		if self.model() == None:
			return None
		self._min = 0
		self._max = 0
		self._minBottomMargin = self._scanValues() + 10
		self._updateRects()


	def _updateRects(self):
		if self.model() == None:
			return None
		self.defineRects()
		metrics = QFontMetrics( self.font() )
		self._yAxis.calculateBounds()
		self._yAxis.origin.setX( metrics.width( str(-1 * self._yAxis.order) ) + metrics.width("0") * self._yAxis.nbDigits + self._marginX )
		self._valuesRect = QRect( self._chartRect )
		self._valuesRect.setX( self._yAxis.origin.x() )
		self._yAxis.xStep = float( self._valuesRect.width() ) / ( self.model().rowCount() )
		metrics = QFontMetrics( self.font() )
		length = 0
		for i in range( self.model().rowCount() ):
			s = str(self.model().headerData( i, Qt.Vertical ))
			length = max( length, metrics.width( s ) )

		self._verticalLabel = (length > self._yAxis.xStep)
		self._yAxis.axisPos = self._valuesRect.topLeft()
		self._yAxis.setAlphaBeta( self._valuesRect.height() )
		delta = self._valuesRect.bottom() - ( self._yAxis.origin.y() + self._minBottomMargin )
		if delta < 0:
			self._valuesRect.setHeight( self._valuesRect.height() + delta )
			self._yAxis.setAlphaBeta( self._valuesRect.height() )
		self._yAxis.xAxisLength = self._valuesRect.width()
		self._yAxis.valueCount = self.model().rowCount()
		self._titleRect.moveTo( self._chartRect.bottomLeft() )
		self._titleRect.translate( (self._chartRect.width() - self._titleRect.width())/2, 20 )


