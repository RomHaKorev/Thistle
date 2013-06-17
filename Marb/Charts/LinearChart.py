from ..Global import Color, Type
from .Chart import Chart, ChartStyle
from .Delegates import PointDelegate, BarDelegate

from PySide.QtGui import QPainter, QPen, QStyleOptionViewItem, QColor, QFontMetrics
from PySide.QtCore import QSize, QRect, QPointF, QPoint, Qt

class LinearChart(Chart):
	''' LinearChart class provides a view for QAbstractItemModel to display data in each column as line chart or bar chart.
	
	To each column in the model corresponds a serie of data.
	'''
	def __init__(self, parent=None):
		super(LinearChart, self).__init__( parent )
		self._origin = QPointF(20, 10)
		self._x = 0
		self._pointDelegate = PointDelegate( self )
		self._barDelegate = BarDelegate( self )
		self._minBottomMargin = 0


	def _processSpec(self):
		if self.model() == None:
			return None
		self._chartRect = QRect( QPoint(self._marginX, self._marginY),  self.size() - QSize( self._marginX*2, self._marginY*2 ) )
		metrics = QFontMetrics( self.font() )
		
		self.calculateLegendRect()
		self._chartRect.setHeight( self._chartRect.height() - self._legendRect.height() - 10 )
		self._chartRect.translate( 0, self._legendRect.height() + 10 )
		
		if self._title != "":
			font = self.font()
			font.setItalic( True )
			m = QFontMetrics( font )
			r = QRect( 0, 0, self._chartRect.width() - 40, 0 )
			self._titleRect = m.boundingRect( r, Qt.AlignHCenter | Qt.AlignTop | Qt.TextWordWrap, self._title )
		
			self._chartRect.setHeight( self._chartRect.height() - self._titleRect.height() - 20 )
			
		self._calculateBounds()
		self._origin.setX( metrics.width( str(-1 * self._order) ) + metrics.width("0") * self._nbDigits + self._marginX )
		
		self._valuesRect = QRect( self._chartRect )
		
		self._valuesRect.setX( self._origin.x() )
		self._x = float( self._valuesRect.width() ) / ( self.model().rowCount() )
		
		self._setAlphaBeta()
		delta = self._valuesRect.bottom() - ( self._origin.y() + self._minBottomMargin )
		
		if delta < 0:
			self._valuesRect.setHeight( self._valuesRect.height() + delta )
			self._setAlphaBeta()
				
		self._titleRect.moveTo( self._chartRect.bottomLeft() )
		self._titleRect.translate( (self._chartRect.width() - self._titleRect.width())/2, 20 )
		
		
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

	def process( self ):
		if self.model() == None:
			return None
		self._min = 0
		self._max = 0
		
		self._minBottomMargin = self._scanValues() + 10
		
		self._processSpec()


	def columnType(self, column):
		'''Returns the type for the given column.
		See Marb.Global.Type and setColumnType()
		'''
		if column in self._style:
			return self._style[ column ].type()

		return ChartStyle().type()


	def _barStyleColumns(self):
		'''Returns the list of column with the type Bar. Used to calculate the width of each bar.
		See Marb.Global.Type and setColumnType(), columnType()
		'''
		bars = []
		for c in range( 0, self.model().columnCount() ):
			if self.columnType( c ) == Type.Bar:
				bars.append( c )
		return bars


	def itemRect(self, index ):
		'''Overloaded method.
		'''
		r = QRect()
		t = self.columnType( index.column() )
		value = index.data()
		if value == None:
			return QRect()
		y = self.valueToPx(value)
		x = self._origin.x() + index.row() *  self._x
		space = self._x * 0.2
		orderedColumns = self._calculateOrderedColumn()
		
		if t == Type.Bar:
			bars = self._barStyleColumns()
			w = float(self._x) / len(bars)
			x += w * orderedColumns.index( index.column() )
			tl = QPoint( x + space/2, y )             # top left
			br = QPoint( x + w, self._origin.y() )    # bottom right
			r = QRect( tl, br )
			if value < 0:
				r.translate( 0, 1 )
			else:
				r.translate( 0, -2 )
		else:
			r = QRect( -5, -5, 10 ,10 ).translated( x + self._x/2, y ) 
		return r.normalized()
		

	def paintChart(self, painter):
		'''Overloaded method.
		'''
		painter.setRenderHints( QPainter.Antialiasing | QPainter.TextAntialiasing )

		self._paintAxis(painter)
		ordered = self._calculateOrderedColumn()
		for c in ordered:
			self._paintValues( painter, c )
		
		self._paintTextAxis(painter)
				
		self._paintLegend(painter)
		
		font = self.font()
		font.setItalic( True )
		painter.setFont( font )
		painter.drawText( self._titleRect, Qt.AlignHCenter | Qt.AlignTop | Qt.TextWordWrap, self._title )


	def _paintAxis(self, painter):
		painter.save()
		painter.setPen( QPen( QColor(Color.LightGray), 1.5 ) )
		
		self._paintXAxis(painter)
		self._paintYAxis(painter)
		
		painter.restore()


	def _paintXAxis(self, painter):		
		p1 = QPoint( self._origin.x(), self._origin.y() )
		p2 = p1 + QPoint( self._valuesRect.width() + 6, 0 )
		painter.drawLine( p1, p2 )
		
		x = self._x + self._origin.x()
		i = 0
		while (i < self.model().rowCount() ):
			p1 = QPoint( x, self._origin.y() - 3  )
			p2 = p1 + QPoint( 0, 6 )		
			painter.drawLine( p1, p2 )
			i += 1
			x += self._x


	def _paintYAxis(self, painter):	
		p1 = QPoint( self._origin.x(), self._chartRect.y() )
		p2 = QPoint( self._origin.x(), self._chartRect.bottom() )
		painter.drawLine( p1, p2 )
		
		painter.save()
		c = painter.pen().color()
		c.setAlpha( 150 )
		painter.setPen( QPen( c , 1 ) )
		y = self._minBound
		
		while y <= self._maxBound:
			p1 = QPoint( self._origin.x(), self.valueToPx(y)  )
			p2 = p1 + QPoint( self._valuesRect.width(), 0 )
			painter.drawLine( p1, p2 )
			y += self._tickSize
		painter.restore()


	def _paintTextAxis(self, painter):
		painter.save()
		metrics = QFontMetrics( self.font() )
		h = metrics.height()
		textPos = QPoint( h/2 , self._origin.y() + 5 );

		x = self._x + self._origin.x()
		i = 0
		while (i < self.model().rowCount() ):
			p1 = QPoint( x, self._origin.y() - 3  )
			s = str(self.model().headerData( i, Qt.Vertical ))
			painter.save()
			painter.setPen( QPen( QColor(Color.DarkGray), 1.5 ) )
			painter.rotate( -90 )
			painter.translate( -textPos.y() - metrics.width( s ) , p1.x() - self._x + h )
			painter.drawText( 0, 0, s )
			painter.restore()
			i += 1
			x += self._x

		y = self._minBound
			
		while y <= self._maxBound:
			p1 = QPoint( self._origin.x(), self.valueToPx(y)  )
			s = str(round(y, self._nbDigits))
			s = s.rstrip("0")
			s = s.rstrip(".")
			r = QRect( QPoint( 0, p1.y() - h/2 ), QSize( self._origin.x() - 5 ,h) )
			painter.drawText( r, Qt.AlignRight, s )
			y += self._tickSize
		
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

		for r in range(0, rows):
			index = self.model().index( r, column )
			option = QStyleOptionViewItem()
			value = index.data()
			if value < 0:
				option.decorationPosition = QStyleOptionViewItem.Bottom
			else:
				option.decorationPosition = QStyleOptionViewItem.Top

			option.rect = self.itemRect( index )

			if t | Type.Line == t:
				if r < (rows - 1):
					p1 = option.rect.center()
					p2 = self.itemRect( self.model().index( r + 1, column ) ).center()
					painter.drawLine( p1, p2 )

			if delegate != None:
				delegate.paint( painter, option, index )
		painter.restore()


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













